#include "document.h"
#include "document-page.h"
#include "backend.h"
#include "document-loader.h"

Document::Document(QQuickItem *parent) :
  QQuickItem(parent),
  m_loader(0),
  m_doc(0),
  m_state(Document::None) {

  setFlag(ItemHasContents, false);
}

Document::~Document() {
  // We need to emit those manually because init() emits them
  // and init() calls clearPages()
  // emitting the signals in clearPages() will cause a double emission of the signals.
  emit aboutToReset();
  clearPages();
  clearDocument();
}

QString Document::filePath() const {
  return m_filePath;
}

void Document::setFilePath(const QString& filePath) {
  if (m_filePath != filePath) {
    m_filePath = filePath;

    emit filePathChanged();

    setState(Document::Loading);

    emit aboutToReset();
    clearPages();
    clearDocument();

    m_loader = new DocumentLoader;
    QObject::connect(m_loader, SIGNAL(done()), this, SLOT(loaderDone()));
    QObject::connect(m_loader, SIGNAL(error()), this, SLOT(loaderError()));
    m_loader->start(m_filePath);
  }
}

void Document::clearPages() {
  qDeleteAll(m_pages);
  m_pages.clear();
}

void Document::clearDocument() {
  delete m_doc;
  m_doc = 0;

  if (m_loader) {
    QObject::disconnect(m_loader, SIGNAL(done()), this, SLOT(loaderDone()));
    QObject::disconnect(m_loader, SIGNAL(error()), this, SLOT(loaderError()));
    m_loader->stop();
    m_loader->wait();
    m_loader->deleteLater();
    m_loader = 0;
  }

}

void Document::init() {
  if (m_doc) {
    delete m_doc;
  }

  QList<BackendPage *> backends;
  m_doc = m_loader->releaseBackend(backends);

  if (!m_doc) {
    return;
  }

  qreal width = 0;
  qreal height = 0;

  int pages = backends.size();
  for (int x = 0; x < pages; x++) {
    BackendPage *backend = backends[x];

    DocumentPage *page = new DocumentPage(backend, x, QPointF(0, height), this);
    m_pages << page;

    QSizeF size = page->size();
    qreal rectWidth = size.width();
    qreal rectHeight = size.height();

    width = qMax(width, rectWidth);
    height += rectHeight;
  }

  backends.clear();

  qDebug() << "width" << width << "height" << height;

  setWidth(width);
  setHeight(height);

  setState(Document::Loaded);
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

Document::State Document::state() const {
  return m_state;
}

void Document::setState(const Document::State& state) {
  if (m_state != state) {
    m_state = state;
    emit stateChanged();
  }
}

void Document::zoomChanged() {
  foreach (DocumentPage *page, m_pages) {
    page->reset();
  }
}

void Document::loaderError() {
  // TODO:
}

void Document::loaderDone() {
  init();
}
