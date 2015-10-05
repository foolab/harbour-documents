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

void DocumentLoader::start(const QString& fileName, const QString& mimeType) {
  m_running = true;
  m_fileName = fileName;
  m_mimeType = mimeType;
  QThread::start();
}

void DocumentLoader::stop() {
  // We are not locking on purpose.
  m_running = false;
  m_cond.wakeOne();
}

void DocumentLoader::run() {
  m_lock.lock();

  m_doc = Backend::create(m_fileName, m_mimeType);

  if (!m_doc) {
    QMetaObject::invokeMethod(this, "error", Qt::QueuedConnection);
    m_lock.unlock();
    return;
  }

  if (m_doc->isLocked()) {
    QMetaObject::invokeMethod(this, "locked", Qt::QueuedConnection);
    m_cond.wait(&m_lock);
  }

  if (!m_running) {
    m_lock.unlock();
    return;
  }

  if (m_doc->isLocked()) {
    if (!m_doc->unlock(m_pass)) {
      QMetaObject::invokeMethod(this, "error", Qt::QueuedConnection);
      m_lock.unlock();
      return;
    }
  }

  int pages = m_doc->numPages();
  for (int x = 0; x < pages; x++) {
    if (!m_running) {
      m_lock.unlock();
      return;
    }

    BackendPage *backend = m_doc->page(x);
    m_pages << backend;
  }

  QMetaObject::invokeMethod(this, "done", Qt::QueuedConnection);

  m_lock.unlock();
}

void DocumentLoader::unlockDocument(const QString& pass) {
  QMutexLocker l(&m_lock);
  m_pass = pass;
  m_cond.wakeOne();
}

void DocumentLoader::clear() {
  delete m_doc;
  qDeleteAll(m_pages);
  m_pages.clear();
}
