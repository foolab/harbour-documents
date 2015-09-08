#ifndef TILE_CACHE_H
#define TILE_CACHE_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QImage>
#include <poppler/qt5/poppler-qt5.h>

#define TILE_SIZE    512

class PopplerDocument;
class DocumentPage;
class TileRequest;
class CacheItem;

class Tile {
public:
  DocumentPage *page;
  QRectF rect;
  QImage image;
};

class TileCache : public QThread {
  Q_OBJECT

public:
  TileCache(QObject *parent = 0);
  ~TileCache();

  TileRequest *requestTiles(QList<Tile>& tiles);

  void start(qreal dpiX, qreal dpiY);
  void stop();

  void clear();

protected:
  void run();

signals:
  void tileRequestDone(TileRequest *request);

private:
  bool populateTileFromCache(Tile& tile);
  bool populateTileFromCacheLocked(Tile& tile);
  void addToCache(Tile& tile);
  void expireCacheLocked();
  void expireCacheTsLocked(qint64 ts);

  bool m_running;
  qreal m_dpiX;
  qreal m_dpiY;

  QMutex m_lock;
  QWaitCondition m_cond;

  QSet<CacheItem> m_cache;
  QList<TileRequest *> m_requests;
};

#endif /* TILE_CACHE_H */
