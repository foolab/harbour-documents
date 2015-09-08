#ifndef TILE_REQUEST_H
#define TILE_REQUEST_H

#include <QObject>
#include <QMutex>

class Tile;

class TileRequest : public QObject {
  Q_OBJECT

public:
  TileRequest(QObject *parent = 0);
  ~TileRequest();

  void addPending(Tile& pending);
  void addTile(Tile& tile);

  void expire();

  Tile takePending();

  bool isEmpty();
  bool isExpired();
  bool hasPending();

  const QList<Tile> tiles();

signals:
  void tileAdded();

private:
  QList<Tile> m_tiles;
  QList<Tile> m_pending;
  bool m_expired;
  QMutex m_lock;
};

#endif /* TILE_REQUEST_H */
