#include "lo-backend.h"
#include <QDebug>
#define LOK_USE_UNSTABLE_API
#include <LibreOfficeKit/LibreOfficeKitInit.h>
#include <LibreOfficeKit/LibreOfficeKit.hxx>

#define LO_PATH "/home/nemo/instdir/program/"

lok::Office *LoBackend::m_office = 0;

class LoPage : public BackendPage {
public:
  LoPage(lok::Document *doc, int num) :
    m_doc(doc),
    m_num(num),
    m_width(-1),
    m_height(-1) {
    m_doc->getDocumentSize(&m_width, &m_height);
  }

  ~LoPage() {
    m_doc = 0;
    m_num = 0;
  }

  QSizeF size() {
    return QSizeF(m_width, m_height);
  }

  QImage render(qreal dpiX, qreal dpiY, const QRectF& rect) {
    QImage image(rect.width(), rect.height(), QImage::Format_RGB32);
    uchar *bits = image.bits();
    qreal x = rect.x() / dpiX * 1440.0f;
    qreal y = rect.y() / dpiY * 1440.0f;
    qreal w = rect.width() / dpiX * 1440.0f;
    qreal h = rect.height() / dpiY * 1440.0f;

    m_doc->paintTile(bits, rect.width(), rect.height(), x, y, w, h);

    return image;
  }

  void reset() {
    // Nothing
  }

  qreal toPixels(qreal value) const {
    return value / 1440.0f;
  }

private:
  lok::Document *m_doc;
  int m_num;
  long m_width;
  long m_height;
};

LoBackend::LoBackend() :
  m_doc(0) {
  if (!m_office) {
    m_office = lok::lok_cpp_init(LO_PATH);
  }
}

LoBackend::~LoBackend() {
  delete m_doc;
  m_doc = 0;
}

int LoBackend::numPages() {
  return m_doc->getParts();
}

BackendPage *LoBackend::page(int num) {
  return new LoPage(m_doc, num + 1);
}

bool LoBackend::load(const QString& filePath) {
  if (!m_office) {
    return false;
  }

  try {
    m_doc = m_office->documentLoad(filePath.toLocal8Bit().constData());
    if (!m_doc) {
      return false;
    }

    m_doc->initializeForRendering();
  } catch (...) {
    return false;
  }

  return true;
}

bool LoBackend::isLocked() {
  return false;
}

bool LoBackend::unlock(const QString& password) {
  Q_UNUSED(password);

  // This function should never be called.

  return false;
}

ADD_BACKEND(QList<BackendInfo>()
	    << BackendInfo(".doc", "application/msword", 100),
	    LoBackend);
