#include "document-view-page.h"
#include "document.h"
#include "document-page.h"
#include "renderers.h"
#include <QQuickWindow>

DocumentPageView::DocumentPageView(QQuickItem *parent) :
  m_doc(0),
  m_page(-1) {

  setFlag(ItemHasContents, true);
}

DocumentPageView::~DocumentPageView() {

}

void DocumentPageView::init(Document *doc, int page) {
  m_doc = doc;
  m_page = page;

  update();
}

QSGNode *DocumentPageView::updatePaintNode(QSGNode *oldNode,
					   UpdatePaintNodeData *updatePaintNodeData){

  delete oldNode;
  oldNode = 0;

  if (!m_doc || m_page == -1) {
    return oldNode;
  }

  DocumentPage *page = m_doc->page(m_page);

  if (!page) {
    return oldNode;
  }

  page->reset();

  QSizeF size(page->size());
  qreal dpiX = width() / size.width();
  qreal dpiY = height() / size.height();

  QImage image(page->tile(dpiX, dpiY, QRectF(0, 0, width(), height())));
  page->reset();

  QSGSimpleTextureNode *node = new SimpleTextureNode;
  node->setFlag(QSGNode::OwnedByParent, true);
  node->setTexture(window()->createTextureFromImage(image));
  node->setRect(QRectF(0, 0, image.width(), image.height()));
  node->setFiltering(QSGTexture::Nearest);
  return node;
}
