#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QQuickItem>

class DocumentPage;
class Backend;

class Document : public QQuickItem {
  Q_OBJECT

  Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged);
  Q_PROPERTY(qreal documentWidth READ documentWidth NOTIFY documentWidthChanged);
  Q_PROPERTY(qreal documentHeight READ documentHeight NOTIFY documentHeightChanged);
  Q_PROPERTY(qreal zoom READ zoom WRITE setZoom NOTIFY zoomChanged);
  Q_PROPERTY(qreal dpiX READ dpiX NOTIFY dpiXChanged);
  Q_PROPERTY(qreal dpiY READ dpiY NOTIFY dpiYChanged);

public:
  Document(QQuickItem *parent = 0);
  ~Document();

  QString filePath() const;
  void setFilePath(const QString& filePath);

  qreal documentWidth() const;
  qreal documentHeight() const;

  qreal zoom() const;
  void setZoom(qreal zoom);

  qreal dpiX() { return m_dpiX * m_zoom; }
  qreal dpiY() { return m_dpiY * m_zoom; }

  QList<DocumentPage *> findPages(qreal top, qreal bottom);
  DocumentPage *page(int p);

signals:
  void aboutToReset();
  void reset();

  void filePathChanged();
  void documentWidthChanged();
  void documentHeightChanged();
  void zoomChanged();
  void dpiXChanged();
  void dpiYChanged();

private:
  void clear();
  void clearDocument();

  void init();

  QList<DocumentPage *> m_pages;

  Backend *m_doc;
  QString m_filePath;
  qreal m_zoom;
  qreal m_width;
  qreal m_height;
  qreal m_dpiX;
  qreal m_dpiY;
};

#endif /* DOCUMENT_H */
