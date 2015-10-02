#ifndef DOCUMENT_VIEW_PAGE_H
#define DOCUMENT_VIEW_PAGE_H

#include <QQuickItem>

class Document;

class DocumentPageView : public QQuickItem {
  Q_OBJECT

public:
  DocumentPageView(QQuickItem *parent = 0);
  ~DocumentPageView();

public slots:
  void init(Document *doc, int page);

protected:
  QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *updatePaintNodeData);

private:
  Document *m_doc;
  int m_page;
};

#endif /* DOCUMENT_VIEW_PAGE_H */
