#include "document-page.h"
#include <QImage>
#include <QDebug>
#include "backend.h"

DocumentPage::DocumentPage(BackendPage *page, int num, const QPointF& pos, QObject *parent) :
  QObject(parent),
  m_page(page),
  m_num(num),
  m_pos(pos) {

}

DocumentPage::~DocumentPage() {
  delete m_page;
  m_page = 0;
}

QPointF DocumentPage::pos() const {
  return m_pos;
}

QSizeF DocumentPage::size() {
  QSizeF size(m_page->size());

  size.setWidth(size.width() / 72.0f);
  size.setHeight(size.height() / 72.0f);

  return size;
}

QImage DocumentPage::tile(qreal dpiX, qreal dpiY, QRectF& rect) {
  return m_page->render(dpiX, dpiY, rect);
}

int DocumentPage::number() const {
  return m_num;
}

void DocumentPage::reset() {
  if (m_page) {
    m_page->reset();
  }
}
