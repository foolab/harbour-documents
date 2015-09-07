#include "document-view.h"
#include <QPainter>
#include <QDebug>
#include "poppler-document.h"
#include "document-page.h"

#define UPDATE_DELAY 10
#define TILE_SIZE    512
#define EXTRA_TILES  1

DocumentView::DocumentView(QQuickItem *parent) :
  QQuickPaintedItem(parent),
  m_doc(0),
  m_cache(new TileCache(this)),
  m_x(0),
  m_y(0) {

  m_timer.setInterval(UPDATE_DELAY);
  m_timer.setSingleShot(true);

  qRegisterMetaType<Tile>("Tile");

  QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(refreshTiles()));
  QObject::connect(m_cache, SIGNAL(tileAvailable(Tile)),
		   this, SLOT(tileAvailable(Tile)), Qt::QueuedConnection);
}

DocumentView::~DocumentView() {
  m_doc = 0;
  m_cache->stop();
  m_cache->wait();
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

#if 0
void DocumentView::setZoom(qreal zoom) {
  if (!qFuzzyCompare(zoom, m_zoom)) {
    m_zoom = zoom;
    emit zoomChanged();

    init();
  }
}
#endif

void DocumentView::init() {
  if (m_cache->isRunning()) {
    m_cache->stop();
    m_cache->wait();
  }

  m_cache->clear();

  m_cache->setDocument(m_doc);
  m_cache->start();

  m_timer.start();
}

void DocumentView::paint(QPainter *painter) {
  qDebug() << "Dimensions:" << width() << height();
  qDebug() << "Position:" << m_x << m_y;

  //  static int foo = 0;

  foreach (const Tile& tile, m_tiles) {
    QPointF pt = tile.rect.topLeft() - QPointF(m_x, m_y);

    //    qDebug() << "Rendering tile at" << pt;

    painter->drawImage(QRectF(pt, tile.image.size()), tile.image);

    //tile.image.save(QString("out/%1.jpg").arg(foo++), "jpeg");
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

  xx = qMax(0.0, xx);
  yy = qMax(0.0, yy);

  qDebug() << "rect" << xx << yy << ww << hh;

  QList<DocumentPage *> pages = m_doc->findPages(yy, yy + hh);

  foreach (DocumentPage *page, pages) {
    QRectF pageRect(QPointF(0, page->y()), page->size(m_doc->dpiX(), m_doc->dpiY()));
    //    qDebug() << "page " << p << " rect " << pageRect;

    QRectF rect(xx, yy, ww, hh);

    // TODO: <= ?
    for (int x = pageRect.x(); x < pageRect.right(); x += TILE_SIZE) {
      for (int y = pageRect.y(); y < pageRect.bottom(); y += TILE_SIZE) {
	QRectF r(x, y, TILE_SIZE, TILE_SIZE);
	//	qDebug() << "checking rect " << r;
	if (rect.intersects(r)) {
	  // In case our width or height are greater than those of the page
	  if (r.right() > pageRect.right()) {
	    r.setRight(pageRect.right());
	  }

	  if (r.bottom() > pageRect.bottom()) {
	    r.setBottom(pageRect.bottom());
	  }

	  Tile t;
	  t.y = pageRect.top();
	  t.rect = r;
	  t.page = page;
	  tiles << t;
	  qDebug() << "Added tile " << t.rect;
	}
      }
    }
  }

  m_tiles = m_cache->requestTiles(tiles);
  if (!m_tiles.isEmpty()) {
    update();
  }
}

void DocumentView::tileAvailable(Tile tile) {
  m_tiles << tile;
  update();
}
