#include "tile-cache.h"
#include "poppler-document.h"
#include "document-page.h"
#include <QDebug>

TileCache::TileCache(QObject *parent) :
  QThread(parent),
  m_running(true),
  m_doc(0) {

}

TileCache::~TileCache() {

}

QList<Tile> TileCache::requestTiles(QList<Tile>& tiles) {
  QMutexLocker l(&m_lock);

  QList<Tile> available;

  m_current.clear();

  while (!tiles.isEmpty()) {
    Tile tile = tiles.takeFirst();
    QRectF rect(tile.rect);
    rect.setTop(rect.top() - tile.page->y());

    if (populateTileFromCacheLocked(tile, rect)) {
      available << tile;
    } else {
      m_current << tile;
    }
  }

  if (!m_current.isEmpty()) {
    m_cond.wakeOne();
  }

  return available;
}

void TileCache::start() {
  m_running = true;
  QThread::start();
}

void TileCache::stop() {
  QMutexLocker l(&m_lock);
  m_current.clear();
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
    if (m_current.isEmpty()) {
      m_cond.wait(&m_lock);
      m_lock.unlock();
      continue;
    }

    QList<Tile> current = m_current;
    m_current.clear();
    m_lock.unlock();

    while (!current.isEmpty()) {
      Tile tile = current.takeFirst();
      QRectF rect(tile.rect);
      rect.setTop(rect.top() - tile.page->y());

      // If it's in cache then fetch it from there:
      if (populateTileFromCache(tile, rect)) {
	emit tileAvailable(tile);
      } else {
	// generate it:
	tile.image = tile.page->tile(m_doc->dpiX(), m_doc->dpiY(), rect);
	addToCache(tile);
	emit tileAvailable(tile);
      }

      if (!m_running) {
	return;
      }
    }
  }
}

bool TileCache::populateTileFromCacheLocked(Tile& tile, const QRectF& rect) {
  for (int x = 0; x < m_cache.size(); x++) {
    const Tile& t = m_cache[x];
    QRectF r(t.rect);
    r.setTop(r.top() - t.page->y());

    if (t.page == tile.page && rect == r) {
      tile.image = t.image;
      qDebug() << "Served tile from cache";
      return true;
    }
  }

  return false;
}

bool TileCache::populateTileFromCache(Tile& tile, const QRectF& rect) {
  QMutexLocker l(&m_lock);

  return populateTileFromCacheLocked(tile, rect);
}

void TileCache::addToCache(Tile& tile) {
  QMutexLocker l(&m_lock);
  m_cache << tile;
}
