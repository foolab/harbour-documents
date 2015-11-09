#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QQuickItem>
#include <memory>
#include "documents-fwd.h"

class Document : public QQuickItem {
  Q_OBJECT

  Q_PROPERTY(QString filePath READ filePath NOTIFY filePathChanged);
  Q_PROPERTY(QString mimeType READ mimeType NOTIFY mimeTypeChanged);
  Q_PROPERTY(State state READ state NOTIFY stateChanged);
  Q_PROPERTY(int pageCount READ pageCount NOTIFY pageCountChanged);
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
  QString mimeType() const;

  QList<DocumentPage *> findPages(qreal top, qreal bottom);
  DocumentPage *page(int p);

  State state() const;
  void setState(const State& state);

  int pageCount() const;

  void zoomChanged();

  Q_INVOKABLE qreal pagePosition(int page);

  BackendOutlineHandle outline();

public slots:
  void unlockDocument(const QString& pass);
  void init(const QString& filePath, const QString& mimeType);

signals:
  void aboutToReset();
  void filePathChanged();
  void mimeTypeChanged();
  void stateChanged();
  void pageCountChanged();

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
  QString m_mimeType;
};

#endif /* DOCUMENT_H */
