#include "tile-request.h"
#include "tile-cache.h"

TileRequest::TileRequest(QObject *parent) :
  QObject(parent),
  m_expired(false),
  m_done(false) {

}

TileRequest::~TileRequest() {

}

void TileRequest::addPending(Tile& pending) {
  QMutexLocker l(&m_lock);
  m_pending << pending;
}

void TileRequest::addTile(Tile& tile) {
  QMutexLocker l(&m_lock);
  m_tiles << tile;
  emit tileAdded();
}

void TileRequest::expire() {
  QMutexLocker l(&m_lock);
  m_expired = true;

  if (m_done) {
    deleteLater();
  }
}

void TileRequest::done() {
  QMutexLocker l(&m_lock);
  m_done = true;

  if (m_expired) {
    deleteLater();
  }
}

Tile TileRequest::takePending() {
  QMutexLocker l(&m_lock);
  return m_pending.takeFirst();
}

bool TileRequest::isEmpty() {
  QMutexLocker l(&m_lock);
  return m_tiles.isEmpty();
}

bool TileRequest::isExpired() {
  QMutexLocker l(&m_lock);
  return m_expired;
}

bool TileRequest::hasPending() {
  QMutexLocker l(&m_lock);
  return !m_pending.isEmpty();
}

const QList<Tile> TileRequest::tiles() {
  QMutexLocker l(&m_lock);
  return m_tiles;
}
