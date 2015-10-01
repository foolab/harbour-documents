#ifndef RENDERERS_H
#define RENDERERS_H

#include <QList>

class Tile;
class QSGNode;
class DocumentView;
class QRectF;

class Renderer {
public:
  Renderer(DocumentView *view);
  ~Renderer();

  qint64 cookie() const;
  void setTiles(qint64 cookie, const QList<Tile>& tiles);
  bool addTile(qint64 cookie, const Tile& tile);

  QList<Tile> tiles() const;
  void clearTiles();

  QSGNode *paintNode(const QRectF& viewport) const;

private:
  DocumentView *m_view;
  qint64 m_cookie;
  QList<Tile> m_tiles;
};

#endif /* RENDERERS_H */
