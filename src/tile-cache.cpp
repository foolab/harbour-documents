#include "tile-cache.h"
#include "document-page.h"
#include <QDebug>
#include <QDateTime>

#define TILE_MEM (TILE_SIZE * TILE_SIZE * 4)
#define MAX_MEM (80 * 1024 * 1024) // 80MB
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

TileCache::TileCache(qreal dpiX, qreal dpiY, QObject *parent) :
  QThread(parent),
  m_running(false),
  m_dpiX(dpiX),
  m_dpiY(dpiY) {

  qRegisterMetaType<Tile>("Tile");
}

TileCache::~TileCache() {
  m_requests.clear();
}

QList<Tile> TileCache::requestTiles(QList<Tile>& tiles, qint64 cookie) {
  QMutexLocker l(&m_lock);
  m_requests.clear();

  QList<Tile> result;
  QList<Tile> request;

  foreach (Tile t, tiles) {
    if (populateTileFromCacheLocked(t)) {
      result << t;
    } else {
      request << t;
    }
  }

  if (!request.isEmpty()) {
    m_requests.insert(cookie, request);
    m_cond.wakeOne();
  } else {
    QMetaObject::invokeMethod(this, "requestDone", Qt::QueuedConnection, Q_ARG(qint64, cookie));
  }

  //  qDebug() << Q_FUNC_INFO << cookie;

  return result;
}

void TileCache::start() {
  m_running = true;
  QThread::start();
}

void TileCache::stop() {
  QMutexLocker l(&m_lock);
  m_cond.wakeOne();
  m_requests.clear();
  m_running = false;
}

void TileCache::clear() {
  QMutexLocker l(&m_lock);
  m_cache.clear();
}

void TileCache::run() {
  //  qDebug() << Q_FUNC_INFO;

  m_lock.lock();

  while (m_running) {
    if (m_requests.isEmpty()) {
      // Expire cache:
      expireCacheLocked();
      //      qDebug() << Q_FUNC_INFO << "Waiting for requests";
      m_cond.wait(&m_lock);
      continue;
    }

    qint64 cookie = m_requests.firstKey();
    if (m_requests.first().isEmpty()) {
      m_requests.remove(cookie);
      QMetaObject::invokeMethod(this, "requestDone", Q_ARG(qint64, cookie));
      continue;
    }

    bool tileDone = false;
    Tile tile = findBestTileLocked(m_requests.first(), tileDone);
    m_lock.unlock();
    if (!tileDone) {
      // generate it:
      tile.image = tile.page->tile(m_dpiX, m_dpiY, tile.rect);
      addToCache(tile);
    }
    m_lock.lock();

    QMetaObject::invokeMethod(this, "tileAvailable", Q_ARG(Tile, tile), Q_ARG(qint64, cookie));
  }

  m_lock.unlock();

  //  qDebug() << Q_FUNC_INFO << "Done";
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
    //    qDebug() << "Cache size" << m_cache.size() << "and maximum is" << CACHE_ITEMS;

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

    //    qDebug() << "Cache size" << m_cache.size();
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

Tile TileCache::findBestTileLocked(QList<Tile>& tiles, bool& tileDone) {
  // find a tile that is visible and already in the cache:
  int size = tiles.size();

  for (int x = 0; x < size; x++) {
    Tile tile = tiles[x];
    if (tile.visible && populateTileFromCacheLocked(tile)) {
      tileDone = true;
      tiles.removeAt(x);
      return tile;
    }
  }

  // Nothing. Get a visible tile:
  for (int x = 0; x < size; x++) {
    Tile tile = tiles[x];
    if (tile.visible) {
      tileDone = populateTileFromCacheLocked(tile);
      tiles.removeAt(x);
      return tile;
    }
  }

  // Just return any tile:
  // TODO: We need a way to generate tiles near the center first.
  Tile tile = tiles.takeFirst();
  tileDone = populateTileFromCacheLocked(tile);

  return tile;
}
