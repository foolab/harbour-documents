#ifndef DOCUMENT_PAGE_H
#define DOCUMENT_PAGE_H

#include <QObject>
#include <poppler/qt5/poppler-qt5.h>

class DocumentPage : public QObject {
  Q_OBJECT

public:
  DocumentPage(Poppler::Page *page, int num, qreal y, QObject *parent = 0);
  ~DocumentPage();

  qreal y() const { return m_y; }
  QSizeF size(qreal dpiX, qreal dpiY);
  QImage tile(qreal dpiX, qreal dpiY, QRectF& rect);

  QList<QRectF> segments(int tileSize, qreal dpiX, qreal dpiY);

private:
  Poppler::Page *m_page;
  int m_num;
  qreal m_y;
};

#endif /* DOCUMENT_PAGE_H */
