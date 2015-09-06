#include "poppler-document.h"
#include <QGuiApplication>
#include <QScreen>
#include <QImage>

PopplerDocument::PopplerDocument(QQuickItem *parent) :
  QQuickItem(parent),
  m_doc(0),
  m_width(0),
  m_height(0) {

  setFlag(ItemHasContents, false);

  m_dpiX = QGuiApplication::primaryScreen()->logicalDotsPerInchX();
  m_dpiY = QGuiApplication::primaryScreen()->logicalDotsPerInchY();

  qDebug() << "DPI: x=" << m_dpiX << " y=" << m_dpiY;
}

PopplerDocument::~PopplerDocument() {
  clear();
}

QString PopplerDocument::filePath() const {
  return m_filePath;
}

void PopplerDocument::setFilePath(const QString& filePath) {
  if (m_filePath != filePath) {
    m_filePath = filePath;

    emit filePathChanged();

    // TODO: No need to worry about painting for now
    clear();

    m_doc = Poppler::Document::load(m_filePath);
    if (!m_doc) {
      // TODO: error
    }

    qreal width = 0;
    qreal height = 0;

    int pages = m_doc->numPages();
    for (int x = 0; x < pages; x++) {
      Poppler::Page *page = m_doc->page(x);
      m_pages << page;

      QPointF topLeft(0, height);
      QSizeF size = page->pageSizeF();
      qreal rectWidth = size.width() / 72.0 * m_dpiX;
      qreal rectHeight = size.height() / 72.0 * m_dpiY;

      width = qMax(width, rectWidth);
      height += rectHeight;

      m_rects << QRectF(topLeft, QSizeF(rectWidth, rectHeight));
    }

    qDebug() << "width" << width << "height" << height;

    setWidth(width);
    setHeight(height);

    m_width = width;
    emit documentWidthChanged();

    m_height = height;
    emit documentHeightChanged();
  }
}

void PopplerDocument::clear() {
  delete m_doc;
  m_doc = 0;

  qDeleteAll(m_pages);
  m_pages.clear();
  m_rects.clear();
}

qreal PopplerDocument::documentWidth() const {
  return m_width;
}

qreal PopplerDocument::documentHeight() const {
  return m_height;
}

QImage PopplerDocument::tile(Poppler::Page *page, const QRectF& rect) {
  return page->renderToImage(m_dpiX, m_dpiY, rect.x(), rect.y(), rect.width(), rect.height());
}

QList<int> PopplerDocument::findPages(qreal top, qreal bottom) {
  QList<int> pages;

  QRectF rect(QPointF(0, top), QPointF(1, bottom));
  for (int i = 0; i < m_rects.size(); i++) {
    if (m_rects[i].intersects(rect)) {
      pages << i;
    } else if (!pages.isEmpty()) {
      // First non intersecting page after we found an intersecting one terminates the loop
      break;
    }
  }

  return pages;
}

Poppler::Page *PopplerDocument::page(int p) {
  return m_pages[p];
}

QRectF PopplerDocument::rect(int p) {
  return m_rects[p];
}
