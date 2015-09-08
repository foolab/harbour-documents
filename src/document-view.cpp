#include "document-view.h"
#include <QPainter>
#include <QDebug>
#include "poppler-document.h"
#include "document-page.h"
#include "tile-request.h"

#define UPDATE_DELAY 10
#define EXTRA_TILES  1

DocumentView::DocumentView(QQuickItem *parent) :
  QQuickPaintedItem(parent),
  m_doc(0),
  m_cache(new TileCache(this)),
  m_x(0),
  m_y(0),
  m_request(0) {

  m_timer.setInterval(UPDATE_DELAY);
  m_timer.setSingleShot(true);

  QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(refreshTiles()));
  QObject::connect(m_cache, SIGNAL(tileRequestDone(TileRequest *)),
		   this, SLOT(tileRequestDone(TileRequest *)));
}

DocumentView::~DocumentView() {
  m_doc = 0;
  if (m_cache->isRunning()) {
    m_cache->stop();
    m_cache->wait();
  }

  if (m_request) {
    m_request->expire();
    m_request = 0;
  }
}

PopplerDocument *DocumentView::document() const {
  return m_doc;
}

void DocumentView::setDocument(PopplerDocument *document) {
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
  if (m_cache->isRunning()) {
    m_cache->stop();
    m_cache->wait();
  }

  m_cache->clear();

  if (m_doc) {
    m_cache->start(m_doc->dpiX(), m_doc->dpiY());
    m_timer.start();
  }
}

void DocumentView::paint(QPainter *painter) {
  //  qDebug() << "Dimensions:" << width() << height();
  //  qDebug() << "Position:" << m_x << m_y;

  //  static int foo = 0;

  if (!m_request) {
    return;
  }

  qreal w = width();

  foreach (const Tile& tile, m_request->tiles()) {
    qreal y = tile.rect.top() + tile.page->y() - m_y;

    qreal pageWidth = tile.page->size(m_doc->dpiX(), m_doc->dpiY()).width();
    qreal adjX = 0;

    if (w > pageWidth) {
      adjX = (w - pageWidth) / 2;
    }

    QPointF pt(tile.rect.left() - m_x + adjX, y);

    //    qDebug() << "Rendering tile at" << pt << tile.image.size();

    painter->drawImage(QRectF(pt, tile.image.size()), tile.image);

    //tile.image.save(QString("out/%1.jpg").arg(foo++), "jpeg");
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
	t.page = page;
	tiles << t;
	//	qDebug() << "Added tile " << t.rect;
      }
    }
  }

  if (m_request) {
    QObject::disconnect(m_request, SIGNAL(tileAdded()), this, SLOT(tileAdded()));
    m_request->expire();
  }

  m_request = m_cache->requestTiles(tiles);
  QObject::connect(m_request, SIGNAL(tileAdded()), this, SLOT(tileAdded()));

  if (!m_request->isEmpty()) {
    update();
  }
}

void DocumentView::tileAdded() {
  update();
}

void DocumentView::tileRequestDone(TileRequest *request) {
  if (request->isExpired()) {
    delete request;
  } else {
    Q_ASSERT(request == m_request);
  }
}
