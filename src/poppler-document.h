#ifndef POPPLER_DOCUMENT_H
#define POPPLER_DOCUMENT_H

#include <QQuickItem>
#include <poppler/qt5/poppler-qt5.h>

class PopplerDocument : public QQuickItem {
  Q_OBJECT

  Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged);
  Q_PROPERTY(qreal documentWidth READ documentWidth NOTIFY documentWidthChanged);
  Q_PROPERTY(qreal documentHeight READ documentHeight NOTIFY documentHeightChanged);

public:
  PopplerDocument(QQuickItem *parent = 0);
  ~PopplerDocument();

  QString filePath() const;
  void setFilePath(const QString& filePath);

  qreal documentWidth() const;
  qreal documentHeight() const;

  QList<int> findPages(qreal top, qreal bottom);
  Poppler::Page *page(int p);
  QRectF rect(int p);

  QImage tile(Poppler::Page *page, const QRectF& rect);

signals:
  void filePathChanged();
  void documentWidthChanged();
  void documentHeightChanged();

private:
  void clear();

  QList<Poppler::Page *> m_pages;
  QList<QRectF> m_rects;

  Poppler::Document *m_doc;
  QString m_filePath;
  qreal m_width;
  qreal m_height;
  qreal m_dpiX;
  qreal m_dpiY;
};

#endif /* POPPLER_DOCUMENT_H */
