#include "tile-cache.h"
#include "document-page.h"
#include "tile-request.h"
#include <QDebug>
#include <QDateTime>

#define TILE_MEM (TILE_SIZE * TILE_SIZE * 4)
#define MAX_MEM (20 * 1024 * 1024) // 20MB
#define CACHE_ITEMS (MAX_MEM / TILE_MEM)

struct CacheItem {
  DocumentPage *page;
  QRectF rect;
  QImage image;
  qint64 ts;

  bool operator==(const CacheItem& other) const {
    return rect == other.rect && page->number() == other.page->number();
  }
};

static inline uint qHash(const CacheItem& item) {
  // Ok this is not as good as I want.
  return item.page->number();
}

TileCache::TileCache(QObject *parent) :
  QThread(parent),
  m_running(true),
  m_dpiX(0),
  m_dpiY(0) {

}

TileCache::~TileCache() {
  qDeleteAll(m_requests);
  m_requests.clear();
}

TileRequest *TileCache::requestTiles(QList<Tile>& tiles) {
  QMutexLocker l(&m_lock);

  TileRequest *request = new TileRequest(this);

  while (!tiles.isEmpty()) {
    Tile tile = tiles.takeFirst();
    if (populateTileFromCacheLocked(tile)) {
      request->addTile(tile);
    } else {
      request->addPending(tile);
    }
  }

  m_requests << request;
  m_cond.wakeOne();

  return request;
}

void TileCache::start(qreal dpiX, qreal dpiY) {
  m_dpiX = dpiX;
  m_dpiY = dpiY;
  m_running = true;
  QThread::start();
}

void TileCache::stop() {
  QMutexLocker l(&m_lock);
  qDeleteAll(m_requests);
  m_requests.clear();
  m_running = false;
  m_cond.wakeOne();
}

void TileCache::clear() {
  QMutexLocker l(&m_lock);
  m_cache.clear();
}

void TileCache::run() {
  while (m_running) {
    m_lock.lock();
    if (m_requests.isEmpty()) {
      // Expire cache:
      expireCacheLocked();

      m_cond.wait(&m_lock);
      m_lock.unlock();
      continue;
    }

    TileRequest *request = m_requests.takeFirst();
    m_lock.unlock();

    while (request->hasPending()) {
      if (request->isExpired()) {
	request->done();
	emit tileRequestDone(request);
	request = 0;
	break;
      }

      Tile tile = request->takePending();

      // If it's in cache then fetch it from there:
      if (populateTileFromCache(tile)) {
	request->addTile(tile);
      } else {
	// generate it:
	tile.image = tile.page->tile(m_dpiX, m_dpiY, tile.rect);
	addToCache(tile);
	request->addTile(tile);
      }

      if (!m_running) {
	if (request) {
	  request->done();
	  emit tileRequestDone(request);
	  request = 0;
	}

	return;
      }
    }

    if (request) {
      request->done();
      emit tileRequestDone(request);
      request = 0;
    }
  }
}

bool TileCache::populateTileFromCacheLocked(Tile& tile) {
  CacheItem dummy;
  dummy.rect = tile.rect;
  dummy.page = tile.page;

  QSet<CacheItem>::iterator iter = m_cache.find(dummy);
  if (iter == m_cache.end()) {
    return false;
  }

  // OK, this is bad but we do not use ts for hashing so I hope it will be safe.
  const CacheItem& item = *iter;
  const_cast<CacheItem&>(item).ts = QDateTime::currentMSecsSinceEpoch();
  tile.image = iter->image;

  return true;
}

bool TileCache::populateTileFromCache(Tile& tile) {
  QMutexLocker l(&m_lock);

  return populateTileFromCacheLocked(tile);
}

void TileCache::addToCache(Tile& tile) {
  QMutexLocker l(&m_lock);
  CacheItem item;
  item.page = tile.page;
  item.rect = tile.rect;
  item.image = tile.image;
  item.ts = QDateTime::currentMSecsSinceEpoch();
  m_cache << item;
}

void TileCache::expireCacheLocked() {
  if (m_cache.size() > CACHE_ITEMS) {
    qDebug() << "Cache size" << m_cache.size() << "and maximum is" << CACHE_ITEMS;

    QMap<qint64, int> map;
    for (QSet<CacheItem>::const_iterator iter = m_cache.constBegin();
	 iter != m_cache.constEnd(); iter++) {
      map.insert(iter->ts, 0);
    }

    while (m_cache.size() >= CACHE_ITEMS) {
      if (!map.isEmpty()) {
	qint64 ts = map.firstKey();
	map.remove(ts);
	expireCacheTsLocked(ts);
      } else {
	break;
      }
    }

    qDebug() << "Cache size" << m_cache.size();
  }
}

void TileCache::expireCacheTsLocked(qint64 ts) {
  for (QSet<CacheItem>::iterator iter = m_cache.begin();
       iter != m_cache.end(); iter++) {
    if (iter->ts == ts) {
      iter = m_cache.erase(iter);
      // If we happen to erase the last iter then iter will become end()
      // and we will happily crash when we increment the pointer above
      if (iter == m_cache.end()) {
	return;
      }
    }
  }
}
