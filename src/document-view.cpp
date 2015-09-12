#include "document-view.h"
#include <QPainter>
#include <QDebug>
#include "document.h"
#include "document-page.h"

#define UPDATE_DELAY 1
#define EXTRA_TILES  1

DocumentView::DocumentView(QQuickItem *parent) :
  QQuickPaintedItem(parent),
  m_doc(0),
  m_cache(0),
  m_x(0),
  m_y(0),
  m_cookie(0) {

  setRenderTarget(QQuickPaintedItem::FramebufferObject);

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

void DocumentView::init() {
  deleteCache();

  if (m_doc) {
    createCache();
  }

  m_tiles.clear();
  m_timer.start();
}

void DocumentView::paint(QPainter *painter) {
  //  qDebug() << "Dimensions:" << width() << height();
  //  qDebug() << "Position:" << m_x << m_y;

  if (m_tiles.isEmpty()) {
    return;
  }

  qreal w = width();

  foreach (const Tile& tile, m_tiles) {
    QRectF rect = tileRect(tile);
    if (tile.visible) {
      painter->drawImage(rect, tile.image);
    } else {
      //      qDebug() << "Skip tile painting";
    }
  }
}

void DocumentView::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
  QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);

  if (newGeometry.size() != oldGeometry.size()) {
    m_timer.start();
  }
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

  QList<DocumentPage *> pages = m_doc->findPages(yy, yy + hh);

  foreach (DocumentPage *page, pages) {
    QList<QRectF> rects = page->segments(TILE_SIZE, m_doc->dpiX(), m_doc->dpiY());
    foreach (const QRectF r, rects) {
      if (rect.intersects(r)) {
	Tile t;
	t.rect = r.adjusted(0, -page->y(), 0, -page->y());
	t.rect.setWidth(TILE_SIZE);
	t.rect.setHeight(TILE_SIZE);
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
  m_cache = new TileCache(m_doc->dpiX(), m_doc->dpiY(), this);
  QObject::connect(m_cache, SIGNAL(tileAvailable(const Tile&, qint64)),
		   this, SLOT(tileAvailable(const Tile&, qint64)));
  m_cache->start();
}

QRectF DocumentView::tileRect(const Tile& tile) {
  qreal w = width();

  qreal y = tile.rect.top() + tile.page->y() - m_y;

  qreal pageWidth = tile.page->size(m_doc->dpiX(), m_doc->dpiY()).width();
  qreal adjX = 0;

  if (w > pageWidth) {
    adjX = (w - pageWidth) / 2;
  }

  QPointF pt(tile.rect.left() - m_x + adjX, y);

  return QRectF(pt, tile.rect.size());
}
