#ifndef DOCUMENT_LOADER_H
#define DOCUMENT_LOADER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class Backend;
class BackendPage;

class DocumentLoader : public QThread {
  Q_OBJECT

public:
  DocumentLoader(QObject *parent = 0);
  ~DocumentLoader();

  Backend *releaseBackend(QList<BackendPage *>& pages);

  void start(const QString& fileName, const QString& mimeType);
  void stop();
  void unlockDocument(const QString& pass);

protected:
  void run();

signals:
  void locked();

signals:
  void authenticationRequired();
  void error();
  void done();

private:
  void clear();

  QMutex m_lock;
  QWaitCondition m_cond;

  Backend *m_doc;
  QList<BackendPage *> m_pages;
  bool m_running;
  QString m_fileName;
  QString m_mimeType;
  QString m_pass;
};

#endif /* DOCUMENT_LOADER_H */
