#include "backend.h"
#include <poppler/qt5/poppler-qt5.h>

class PopplerPage : public BackendPage {
public:
  PopplerPage(Poppler::Page *page) :
    m_page(page) {}

  ~PopplerPage() {
    delete m_page;
    m_page = 0;
  }

  QSizeF size() {
    return m_page->pageSizeF();
  }

  QImage render(qreal dpiX, qreal dpiY, const QRectF& rect) {
    return m_page->renderToImage(dpiX, dpiY, rect.x(), rect.y(), rect.width(), rect.height());
  }

private:
  Poppler::Page *m_page;
};

class PopplerBackend : public Backend {
public:
  PopplerBackend() :
    m_doc(0) {

  }

  ~PopplerBackend() {
    delete m_doc;
    m_doc = 0;
  }

  int numPages() {
    return m_doc ? m_doc->numPages() : 0;
  }

  BackendPage *page(int num) {
    return new PopplerPage(m_doc->page(num));
  }

  bool load(const QString& filePath) {
    Q_ASSERT(m_doc == 0);
    m_doc = Poppler::Document::load(filePath);
    return m_doc != 0;
  }

private:
  Poppler::Document *m_doc;
};

Backend *Backend::create(const QString& filePath) {
  Backend *backend = new PopplerBackend;
  if (backend->load(filePath)) {
    return backend;
  }

  delete backend;
  return 0;
}
