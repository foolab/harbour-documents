#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QQuickItem>

class DocumentPage;
class Backend;
class DocumentLoader;

class Document : public QQuickItem {
  Q_OBJECT

  Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged);
  Q_PROPERTY(State state READ state NOTIFY stateChanged);
  Q_ENUMS(State);

public:
  typedef enum {
    None,
    Loaded,
    Locked,
    Loading,
    Error,
  } State;

  Document(QQuickItem *parent = 0);
  ~Document();

  QString filePath() const;
  void setFilePath(const QString& filePath);

  QList<DocumentPage *> findPages(qreal top, qreal bottom);
  DocumentPage *page(int p);

  State state() const;
  void setState(const State& state);

  void zoomChanged();

public slots:
  void unlockDocument(const QString& pass);

signals:
  void aboutToReset();
  void filePathChanged();
  void stateChanged();

private slots:
  void loaderError();
  void loaderDone();
  void loaderLocked();

private:
  void clearPages();
  void clearDocument();
  void stopLoader();
  void init();

  DocumentLoader *m_loader;
  QList<DocumentPage *> m_pages;
  State m_state;
  Backend *m_doc;
  QString m_filePath;
};

#endif /* DOCUMENT_H */
