#include "poppler-backend.h"
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

  void reset() {
    // Nothing
  }

private:
  Poppler::Page *m_page;
};

PopplerBackend::PopplerBackend() :
  m_doc(0) {

}

PopplerBackend::~PopplerBackend() {
  delete m_doc;
  m_doc = 0;
}

int PopplerBackend::numPages() {
  return m_doc ? m_doc->numPages() : 0;
}

BackendPage *PopplerBackend::page(int num) {
  return new PopplerPage(m_doc->page(num));
}

bool PopplerBackend::load(const QString& filePath) {
  Q_ASSERT(m_doc == 0);
  m_doc = Poppler::Document::load(filePath);
  return m_doc != 0;
}

ADD_BACKEND(QList<BackendInfo>() << BackendInfo(".pdf", "application/pdf", 1), PopplerBackend);
