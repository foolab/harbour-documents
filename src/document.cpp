#include "document.h"
#include "document-page.h"
#include "backend.h"

Document::Document(QQuickItem *parent) :
  QQuickItem(parent),
  m_doc(0) {

  setFlag(ItemHasContents, false);
}

Document::~Document() {
  // We need to emit those manually because init() emits them
  // and init() calls clearPages()
  // emitting the signals in clearPages() will cause a double emission of the signals.
  emit aboutToReset();
  clearPages();
  clearDocument();
  emit reset();
}

QString Document::filePath() const {
  return m_filePath;
}

void Document::setFilePath(const QString& filePath) {
  if (m_filePath != filePath) {
    m_filePath = filePath;

    emit filePathChanged();

    emit aboutToReset();
    clearPages();
    clearDocument();
    init();
    emit reset();
  }
}

void Document::clearPages() {
  qDeleteAll(m_pages);
  m_pages.clear();
}

void Document::clearDocument() {
  delete m_doc;
  m_doc = 0;
}

void Document::init() {
  if (m_doc) {
    delete m_doc;
  }

  m_doc = Backend::create(m_filePath);

  if (!m_doc) {
    return;
  }

  qreal width = 0;
  qreal height = 0;

  int pages = m_doc->numPages();
  for (int x = 0; x < pages; x++) {
    BackendPage *backend = m_doc->page(x);

    DocumentPage *page = new DocumentPage(backend, x, QPointF(0, height), this);
    m_pages << page;

    QSizeF size = page->size();
    qreal rectWidth = size.width();
    qreal rectHeight = size.height();

    width = qMax(width, rectWidth);
    height += rectHeight;
  }

  qDebug() << "width" << width << "height" << height;

  setWidth(width);
  setHeight(height);
}

QList<DocumentPage *> Document::findPages(qreal top, qreal bottom) {
  QList<DocumentPage *> pages;
  QRectF rect(QPointF(0, top), QPointF(1, bottom));

  for (int i = 0; i < m_pages.size(); i++) {
    DocumentPage *page = m_pages[i];

    QRectF pageRect(page->pos(), page->size());

    if (pageRect.intersects(rect)) {
      pages << m_pages[i];
    } else if (!pages.isEmpty()) {
      // First non intersecting page after we found an intersecting one terminates the loop
      break;
    }
  }

  return pages;
}

DocumentPage *Document::page(int p) {
  return m_pages[p];
}

void Document::zoomChanged() {
  foreach (DocumentPage *page, m_pages) {
    page->reset();
  }
}
