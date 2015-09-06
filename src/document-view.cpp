#include "document-view.h"
#include <QPainter>
#include <QDebug>
#include "poppler-document.h"

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
    m_doc = document;

    m_cache->stop();
    m_cache->wait();
    m_cache->setDocument(m_doc);
    m_cache->start();

    m_timer.start();
    emit documentChanged();
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

void DocumentView::paint(QPainter *painter) {
  //  qDebug() << Q_FUNC_INFO << m_x << m_y;

  static int foo = 0;

  foreach (const Tile& tile, m_tiles) {
    QPointF pt = tile.rect.topLeft() - QPointF(m_x, m_y);

    //    qDebug() << "Rendering tile at" << pt;

    painter->drawImage(pt, tile.image);

    //    tile.image.save(QString("out/%1.jpg").arg(foo++), "jpeg");
  }
}

void DocumentView::refreshTiles() {
  if (!m_doc) {
    update();
    return;
  }

  QList<Tile> tiles;

  //  qDebug() << "viewport: " << m_x << m_y << width() << height();

  qreal xx = qMax(0.0, m_x - (TILE_SIZE * EXTRA_TILES)),
    yy = qMax(0.0, m_y - (TILE_SIZE * EXTRA_TILES)),
    ww = width() + 2 * (TILE_SIZE * EXTRA_TILES),
    hh = height() + 2 * (TILE_SIZE * EXTRA_TILES);

  //  qDebug() << "tiles rect" << xx << yy << ww << hh;

  QList<int> pages = m_doc->findPages(yy, yy + hh);

  foreach (int p, pages) {
    Poppler::Page *page = m_doc->page(p);
    QRectF pageRect = m_doc->rect(p);
    //    qDebug() << "page " << p << " rect " << pageRect;

    QRectF rect(xx, yy, ww, hh);

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
	  //	  qDebug() << "Added tile " << t.rect;
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
