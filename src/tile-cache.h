#ifndef TILE_CACHE_H
#define TILE_CACHE_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QSet>
#include <QMap>
#include <QImage>

#define TILE_SIZE    512

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
  TileCache(qreal dpiX, qreal dpiY, QObject *parent = 0);
  ~TileCache();

  void requestTiles(QList<Tile>& tiles, qint64 cookie);

  void start();
  void stop();
  void clear();

protected:
  void run();

signals:
  void tileAvailable(const Tile& tile, qint64 cookie);

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
  QMap<qint64, QList<Tile> > m_requests;
};

Q_DECLARE_METATYPE(Tile);

#endif /* TILE_CACHE_H */
