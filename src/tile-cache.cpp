#include "tile-cache.h"
#include "poppler-document.h"
#include "document-page.h"
#include "tile-request.h"
#include <QDebug>

TileCache::TileCache(QObject *parent) :
  QThread(parent),
  m_running(true),
  m_doc(0) {

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

void TileCache::start() {
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

void TileCache::setDocument(PopplerDocument *document) {
  m_doc = document;
}

void TileCache::run() {
  while (m_running) {
    m_lock.lock();
    if (m_requests.isEmpty()) {
      m_cond.wait(&m_lock);
      m_lock.unlock();
      continue;
    }

    TileRequest *request = m_requests.takeFirst();
    m_lock.unlock();

    while (request->hasPending()) {
      if (request->isExpired()) {
	request->deleteLater();
	break;
      }

      Tile tile = request->takePending();

      // If it's in cache then fetch it from there:
      if (populateTileFromCache(tile)) {
	request->addTile(tile);
      } else {
	// generate it:
	tile.image = tile.page->tile(m_doc->dpiX(), m_doc->dpiY(), tile.rect);
	addToCache(tile);
	request->addTile(tile);
      }

      if (!m_running) {
	return;
      }
    }
  }
}

bool TileCache::populateTileFromCacheLocked(Tile& tile) {
  for (int x = 0; x < m_cache.size(); x++) {
    const Tile& t = m_cache[x];

    if (t.page == tile.page && tile.rect == t.rect) {
      tile.image = t.image;
      qDebug() << "Served tile from cache";
      return true;
    }
  }

  return false;
}

bool TileCache::populateTileFromCache(Tile& tile) {
  QMutexLocker l(&m_lock);

  return populateTileFromCacheLocked(tile);
}

void TileCache::addToCache(Tile& tile) {
  QMutexLocker l(&m_lock);
  m_cache << tile;
}
