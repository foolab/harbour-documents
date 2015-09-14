#ifndef DOCUMENT_PAGE_H
#define DOCUMENT_PAGE_H

#include <QObject>
#include <QPointF>

class BackendPage;

class DocumentPage : public QObject {
  Q_OBJECT

public:
  DocumentPage(BackendPage *page, int num, const QPointF& pos, QObject *parent = 0);
  ~DocumentPage();

  QPointF pos() const;
  QSizeF size();
  QImage tile(qreal dpiX, qreal dpiY, QRectF& rect);
  int number() const;
  void reset();

private:
  BackendPage *m_page;
  int m_num;
  QPointF m_pos;
};

#endif /* DOCUMENT_PAGE_H */
