#include "renderers.h"
#include "tile-cache.h"
#include "document-view.h"
#include <QSGNode>
#include <QSGSimpleTextureNode>
#include <QQuickWindow>
#include <QDebug>

class SimpleTextureNode : public QSGSimpleTextureNode {
public:
  ~SimpleTextureNode() {
    QSGTexture *t = texture();
    delete t;
  }
};

Renderer::Renderer(DocumentView *view) :
  m_view(view),
  m_cookie(0) {

}

Renderer::~Renderer() {
  clearTiles();
}

qint64 Renderer::cookie() const {
  return m_cookie;
}

void Renderer::setTiles(qint64 cookie, const QList<Tile>& tiles) {
  m_cookie = cookie;
  m_tiles = tiles;
}

bool Renderer::addTile(qint64 cookie, const Tile& tile) {
  if (m_cookie == cookie) {
    m_tiles << tile;

    return true;
  }

  return false;
}

QList<Tile> Renderer::tiles() const {
  return std::move(m_tiles);
}

void Renderer::clearTiles() {
  m_tiles.clear();
}

QSGNode *Renderer::paintNode(const QRectF& viewport) const {
  if (m_tiles.isEmpty()) {
    return 0;
  }

  QSGNode *parentNode = new QSGNode;

  for (const Tile& tile : m_tiles) {
    if (tile.visible) {
      QRectF rect = m_view->tileRect(tile);
      QSGSimpleTextureNode *node = new SimpleTextureNode;
      node->setFlag(QSGNode::OwnedByParent, true);
      node->setTexture(m_view->window()->createTextureFromImage(tile.image));
      node->setRect(rect);
      node->setFiltering(QSGTexture::Nearest);
      //      node->setOwnsTexture(true); // TODO: Qt 5.4

      if (rect.bottom() > viewport.bottom()) {
	QSGClipNode *clip = new QSGClipNode;
	clip->setIsRectangular(true);
	clip->setClipRect(rect.adjusted(0, 0, 0, viewport.bottom() - rect.bottom()));
	parentNode->appendChildNode(clip);
	clip->appendChildNode(node);
	//	qDebug() << "Clipping";
      } else {
	parentNode->appendChildNode(node);
      }
    }
  }

  return parentNode;
}
