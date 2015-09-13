#include "document-page.h"
#include <QImage>
#include <QDebug>
#include "backend.h"

DocumentPage::DocumentPage(BackendPage *page, int num, qreal y, QObject *parent) :
  QObject(parent),
  m_page(page),
  m_num(num),
  m_y(y) {

}

DocumentPage::~DocumentPage() {
  delete m_page;
  m_page = 0;
}

QSizeF DocumentPage::size(qreal dpiX, qreal dpiY) {
  QSizeF size = m_page->size();

  size.setWidth(size.width() / 72.0 * dpiX);
  size.setHeight(size.height() / 72.0 * dpiY);

  return size;
}

QImage DocumentPage::tile(qreal dpiX, qreal dpiY, QRectF& rect) {
  return m_page->render(dpiX, dpiY, rect);
}
