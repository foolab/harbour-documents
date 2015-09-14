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

  QList<DocumentPage *> findPages(qreal top, qreal bottom);
  DocumentPage *page(int p);

  void zoomChanged();

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
};

#endif /* DOCUMENT_H */
