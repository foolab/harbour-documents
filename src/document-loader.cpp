#include "document-loader.h"
#include "backend.h"

DocumentLoader::DocumentLoader(QObject *parent) :
  QThread(parent) {

}

DocumentLoader::~DocumentLoader() {
  clear();
}

Backend *DocumentLoader::releaseBackend(QList<BackendPage *>& pages) {
  std::swap(pages, m_pages);
  Backend *doc = m_doc;
  m_doc = 0;
  return doc;
}

void DocumentLoader::start(const QString& fileName) {
  m_running = true;
  m_fileName = fileName;
  QThread::start();
}

void DocumentLoader::stop() {
  QMutexLocker l(&m_lock);
  m_running = false;
  m_cond.wakeOne();
}

void DocumentLoader::run() {
  m_doc = Backend::create(m_fileName);

  if (!m_doc) {
    QMetaObject::invokeMethod(this, "error", Qt::QueuedConnection);
    return;
  }

  int pages = m_doc->numPages();
  for (int x = 0; x < pages; x++) {
    BackendPage *backend = m_doc->page(x);
    m_pages << backend;
  }

  QMetaObject::invokeMethod(this, "done", Qt::QueuedConnection);
}

void DocumentLoader::authenticate(const QString& user, const QString& pass) {
  // TODO:
}

void DocumentLoader::clear() {
  delete m_doc;
  qDeleteAll(m_pages);
  m_pages.clear();
}
