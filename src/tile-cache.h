#ifndef TILE_CACHE_H
#define TILE_CACHE_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QImage>
#include <poppler/qt5/poppler-qt5.h>

class PopplerDocument;

class Tile {
public:
  Poppler::Page *page;
  QRectF rect;
  QImage image;
  qreal y;
};

class TileCache : public QThread {
  Q_OBJECT

public:
  TileCache(QObject *parent = 0);
  ~TileCache();

  QList<Tile> requestTiles(QList<Tile>& tiles);

  void setDocument(PopplerDocument *document);

  void start();
  void stop();

protected:
  void run();

signals:
  void tileAvailable(Tile tile);

private:
  bool populateTileFromCache(Tile& tile, const QRectF& rect);
  bool populateTileFromCacheLocked(Tile& tile, const QRectF& rect);
  void addToCache(Tile& tile);

  bool m_running;
  PopplerDocument *m_doc;

  QMutex m_lock;
  QWaitCondition m_cond;

  QList<Tile> m_cache;
  QList<Tile> m_current;
};

Q_DECLARE_METATYPE(Tile)

#endif /* TILE_CACHE_H */
