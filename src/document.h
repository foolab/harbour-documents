#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QQuickItem>

class DocumentPage;
class Backend;

class Document : public QQuickItem {
  Q_OBJECT

  Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged);

public:
  Document(QQuickItem *parent = 0);
  ~Document();

  QString filePath() const;
  void setFilePath(const QString& filePath);

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

private:
  void clearPages();
  void clearDocument();

  void init();

  QList<DocumentPage *> m_pages;

  Backend *m_doc;
  QString m_filePath;
  qreal m_zoom;
  qreal m_dpiX;
  qreal m_dpiY;
};

#endif /* DOCUMENT_H */
