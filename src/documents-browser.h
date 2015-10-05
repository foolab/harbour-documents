#ifndef DOCUMENTS_BROWSER_H
#define DOCUMENTS_BROWSER_H

#include <QObject>
#include <QThread>
#include <QStringList>
#include <QAbstractListModel>
#include <deque>

class DocumentsBrowserWorker;
class MimeResolver;
class DocumentsBrowserModel;

class DocumentsBrowser : public QObject {
  Q_OBJECT
  Q_PROPERTY(QObject* model READ model NOTIFY modelChanged);
  Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged);

public:
  DocumentsBrowser(QObject *parent = 0);
  ~DocumentsBrowser();

  QObject *model();

  bool isBusy() const;

public slots:
  void refresh(bool accurateMimeTypes, bool indexSdCards);

signals:
  void modelChanged();
  void busyChanged();

private slots:
  void stopWorker();

private:
  DocumentsBrowserModel *m_model;
  DocumentsBrowserWorker *m_worker;
};

class DocumentsBrowserWorker : public QThread {
  Q_OBJECT

public:
  DocumentsBrowserWorker();
  ~DocumentsBrowserWorker();

  void start(bool indexSdCards, MimeResolver *resolver);
  void stop();

protected:
  void run();

signals:
  void fileAvailable(const QString& file, const QString& mime);
  void done();

private:
  void scanDirectory(const QString& dir);
  void clearResolver();

  bool m_running;
  QStringList m_dirs;
  MimeResolver *m_resolver;
};

class DocumentsBrowserModel : public QAbstractListModel {
  Q_OBJECT

public:
  enum {
    NameRole = Qt::UserRole + 1,
    PathRole = Qt::UserRole + 2,
    MimeRole = Qt::UserRole + 3,
  };

  DocumentsBrowserModel(QObject *parent = 0);

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  QHash<int, QByteArray> roleNames() const;

  void clear();

public slots:
  void fileAvailable(const QString& file, const QString& mime);

private:
  std::deque<std::pair<QString, QString> > m_info;
};

#endif /* DOCUMENTS_BROWSER_H */
