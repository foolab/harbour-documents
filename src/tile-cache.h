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

  void setDocument(PopplerDocument *document);

  void start();
  void stop();

  void clear();

protected:
  void run();

signals:
  void tileAvailable(Tile tile);

private:
  bool populateTileFromCache(Tile& tile);
  bool populateTileFromCacheLocked(Tile& tile);
  void addToCache(Tile& tile);

  bool m_running;
  PopplerDocument *m_doc;

  QMutex m_lock;
  QWaitCondition m_cond;

  QList<Tile> m_cache;
  QList<TileRequest *> m_requests;
};

Q_DECLARE_METATYPE(Tile)

#endif /* TILE_CACHE_H */
