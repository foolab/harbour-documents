#include "poppler-document.h"
#include "document-page.h"
#include <QGuiApplication>
#include <QScreen>

PopplerDocument::PopplerDocument(QQuickItem *parent) :
  QQuickItem(parent),
  m_doc(0),
  m_zoom(1.0),
  m_width(0),
  m_height(0) {

  setFlag(ItemHasContents, false);

  m_dpiX = QGuiApplication::primaryScreen()->physicalDotsPerInchX();
  m_dpiY = QGuiApplication::primaryScreen()->physicalDotsPerInchY();

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

    emit aboutToReset();
    init();
    emit reset();
  }
}

qreal PopplerDocument::zoom() const {
  return m_zoom;
}

void PopplerDocument::setZoom(qreal zoom) {
  if (!qFuzzyCompare(zoom, m_zoom)) {
    m_zoom = zoom;

    emit aboutToReset();
    init();
    emit reset();
  }
}

void PopplerDocument::clear() {
  delete m_doc;
  m_doc = 0;

  qDeleteAll(m_pages);
  m_pages.clear();
}

void PopplerDocument::init() {
  // TODO: No need to worry about painting for now
  clear();

  m_doc = Poppler::Document::load(m_filePath);

  if (!m_doc) {
    return;
  }

  qreal width = 0;
  qreal height = 0;

  int pages = m_doc->numPages();
  for (int x = 0; x < pages; x++) {
    DocumentPage *page = new DocumentPage(m_doc->page(x), x, height, this);
    m_pages << page;

    QSizeF size = page->size(dpiX(), dpiY());
    qreal rectWidth = size.width();
    qreal rectHeight = size.height();

    width = qMax(width, rectWidth);
    height += rectHeight;
  }

  qDebug() << "width" << width << "height" << height;

  setWidth(width);
  setHeight(height);

  m_width = width;
  emit documentWidthChanged();

  m_height = height;
  emit documentHeightChanged();
}

qreal PopplerDocument::documentWidth() const {
  return m_width;
}

qreal PopplerDocument::documentHeight() const {
  return m_height;
}

QList<DocumentPage *> PopplerDocument::findPages(qreal top, qreal bottom) {
  QList<DocumentPage *> pages;
  QRectF rect(QPointF(0, top), QPointF(1, bottom));

  for (int i = 0; i < m_pages.size(); i++) {
    DocumentPage *page = m_pages[i];
    qreal y = page->y();

    QSizeF size(page->size(dpiX(), dpiY()));

    QRectF pageRect(QPointF(0, y), size);

    if (pageRect.intersects(rect)) {
      pages << m_pages[i];
    } else if (!pages.isEmpty()) {
      // First non intersecting page after we found an intersecting one terminates the loop
      break;
    }
  }

  return pages;
}

DocumentPage *PopplerDocument::page(int p) {
  return m_pages[p];
}
