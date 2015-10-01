#include "document-view.h"
#include <QGuiApplication>
#include <QScreen>
#include <QSGNode>
#include <QSGSimpleTextureNode>
#include <QQuickWindow>
#include <QDebug>
#include "document.h"
#include "document-page.h"

#define UPDATE_DELAY 1
#define EXTRA_TILES  2

class SimpleTextureNode : public QSGSimpleTextureNode {
public:
  ~SimpleTextureNode() {
    QSGTexture *t = texture();
    delete t;
  }
};

DocumentView::DocumentView(QQuickItem *parent) :
  QQuickItem(parent),
  m_doc(0),
  m_cache(0),
  m_x(0),
  m_y(0),
  m_zoom(0.0f),
  m_cookie(0) {

  m_dpiX = QGuiApplication::primaryScreen()->physicalDotsPerInchX();
  m_dpiY = QGuiApplication::primaryScreen()->physicalDotsPerInchY();

  //  qDebug() << "DPI: x=" << m_dpiX << " y=" << m_dpiY;

  setFlag(ItemHasContents, true);

  m_timer.setInterval(UPDATE_DELAY);
  m_timer.setSingleShot(true);

  QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(refreshTiles()));
}

DocumentView::~DocumentView() {
  m_doc = 0;
  deleteCache();
  m_tiles.clear();
}

Document *DocumentView::document() const {
  return m_doc;
}

void DocumentView::setDocument(Document *document) {
  if (m_doc != document) {

    if (m_doc) {
      QObject::disconnect(m_doc, SIGNAL(aboutToReset()), this, SLOT(init()));
    }

    m_doc = document;

    if (m_doc) {
      QObject::connect(m_doc, SIGNAL(aboutToReset()), this, SLOT(init()));
    }

    emit documentChanged();

    init();

    resetZoom();
  }
}

qreal DocumentView::contentX() const {
  return m_x;
}

void DocumentView::setContentX(qreal x) {
  if (!qFuzzyCompare(m_x, x)) {
    m_x = x;
    m_timer.start();
    emit contentXChanged();
  }
}

qreal DocumentView::contentY() const {
  return m_y;
}

void DocumentView::setContentY(qreal y) {
  if (!qFuzzyCompare(m_y, y)) {
    m_y = y;
    m_timer.start();
    emit contentYChanged();
  }
}

qreal DocumentView::dpiX() const {
  return m_dpiX * m_zoom;
}

qreal DocumentView::dpiY() const {
  return m_dpiY * m_zoom;
}

qreal DocumentView::zoom() const {
  return m_zoom;
}

void DocumentView::setZoom(qreal zoom) {
  if (!qFuzzyCompare(m_zoom, zoom)) {
    m_zoom = zoom;
    deleteCache();

    if (m_doc) {
      m_doc->zoomChanged();
      init();
    }

    emit zoomChanged();
    emit dpiChanged();
  }
}

void DocumentView::init() {
  deleteCache();

  if (m_doc) {
    createCache();
  }

  m_tiles.clear();
  m_timer.start();
}

void DocumentView::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
  QQuickItem::geometryChanged(newGeometry, oldGeometry);

  if (newGeometry.size() != oldGeometry.size()) {
    m_timer.start();
  }

  resetZoom();
}

QSGNode *DocumentView::updatePaintNode(QSGNode *oldNode,
				       UpdatePaintNodeData *updatePaintNodeData) {

  if (oldNode) {
    delete oldNode;
    oldNode = 0;
  }

  if (m_tiles.isEmpty()) {
    return oldNode;
  }

  QSGClipNode *clip = new QSGClipNode;
  clip->setIsRectangular(true);
  clip->setClipRect(QRectF(0, 0, width(), height()));
  oldNode = clip;

  foreach (const Tile& tile, m_tiles) {
    if (tile.visible) {
      QRectF rect = tileRect(tile);
      QSGSimpleTextureNode *node = new SimpleTextureNode;
      node->setFlag(QSGNode::OwnedByParent, true);
      node->setTexture(window()->createTextureFromImage(tile.image));
      node->setRect(rect);
      node->setFiltering(QSGTexture::Nearest);
      //      node->setOwnsTexture(true); // TODO: Qt 5.4
      oldNode->appendChildNode(node);
    }
  }

  return oldNode;
}

void DocumentView::refreshTiles() {
  if (!m_doc) {
    update();
    return;
  }

  QList<Tile> tiles;

  QRectF viewport(0, 0, width(), height());

  qreal extra = TILE_SIZE * EXTRA_TILES;

  qreal xx = m_x - extra,
    yy = m_y - extra,
    ww = width() + 2 * extra,
    hh = height() + 2 * extra;

  xx = qMax((qreal)0, xx);
  yy = qMax((qreal)0, yy);

  QRectF rect(xx, yy, ww, hh);

  //  qDebug() << "rect" << xx << yy << ww << hh;

  QList<DocumentPage *> pages = m_doc->findPages(yy/dpiY(), (yy + hh)/dpiY());

  foreach (DocumentPage *page, pages) {
    QList<QRectF> rects = pageRectangles(page);
    qreal pos = page->pos().y() * dpiY();

    foreach (const QRectF r, rects) {
      QRectF adjusted(r);
      adjusted.setTop(r.top() + page->pos().y() * dpiY());
      if (rect.intersects(adjusted)) {
	Tile t;
	t.rect = r;
	t.page = page;
	t.visible = viewport.intersects(tileRect(t));
	tiles << t;
	//	qDebug() << "Added tile " << t.rect;
      }
    }
  }

  ++m_cookie;
  m_tiles = m_cache->requestTiles(tiles, m_cookie);
  if (!m_tiles.isEmpty()) {
    update();
  }
}

void DocumentView::tileAvailable(const Tile& tile, qint64 cookie) {
  if (cookie == m_cookie) {
    m_tiles << tile;

    if (tile.visible) {
      //      qDebug() << "calling update";
      update();
    } else {
      //      qDebug() << "skipping update";
    }
  } else {
    //    qDebug() << "Skipping old tile";
  }
}

void DocumentView::tileRequestDone(qint64 cookie) {
  Q_UNUSED(cookie);

  // qDebug() << Q_FUNC_INFO << cookie;
}

void DocumentView::deleteCache() {
  //  qDebug() << Q_FUNC_INFO << m_cache;

  if (m_cache && m_cache->isRunning()) {
    //    qDebug() << Q_FUNC_INFO << "is running";
    m_cache->stop();
    //    qDebug() << Q_FUNC_INFO << "stopped";
    m_cache->wait();
    //    qDebug() << Q_FUNC_INFO << "wait done";
    delete m_cache;
  } else if (m_cache) {
    delete m_cache;
  }

  m_cache = 0;
}

void DocumentView::createCache() {
  m_cache = new TileCache(dpiX(), dpiY(), this);
  QObject::connect(m_cache, SIGNAL(tileAvailable(const Tile&, qint64)),
		   this, SLOT(tileAvailable(const Tile&, qint64)));
  QObject::connect(m_cache, SIGNAL(requestDone(qint64)),
		   this, SLOT(tileRequestDone(qint64)));
  m_cache->start();
}

QRectF DocumentView::tileRect(const Tile& tile) {
  qreal w = width();

  qreal y = tile.rect.top() + (tile.page->pos().y() * dpiY()) - m_y;

  qreal pageWidth = tile.page->size().width() * dpiX();
  qreal adjX = 0;

  if (w > pageWidth) {
    adjX = (w - pageWidth) / 2;
  }

  QPointF pt(tile.rect.left() - m_x + adjX, y);

  return QRectF(pt, tile.rect.size());
}

QList<QRectF> DocumentView::pageRectangles(DocumentPage *page) {
  QSizeF size(page->size());
  size.setWidth(size.width() * dpiX());
  size.setHeight(size.height() * dpiY());

  QList<QRectF> rects;
  for (int y = 0; y < size.height(); y += TILE_SIZE) {
    for (int x = 0; x < size.width(); x += TILE_SIZE) {
      QRectF rect(x, y, TILE_SIZE, TILE_SIZE);
      //      qDebug() << "Rect before" << rect;
      rect.setRight(qMin(rect.right(), size.width()));
      rect.setBottom(qMin(rect.bottom(), size.height()));
      //      qDebug() << "Rect after" << rect;

      rects << rect;
    }
  }

  return rects;
}

void DocumentView::resetZoom() {
  if (m_zoom == 0 && m_doc && width() > 0) {
    // final width = m_doc->width() * m_zoom * m_dpiX
    qreal factor = width() / m_doc->width() / m_dpiX;
    setZoom(factor);
  }
}
