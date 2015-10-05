#include "documents-browser.h"
#include <QFileInfo>
#include <QDirIterator>
#include <QDir>
#include <QMimeDatabase>
#include <QMimeType>
#include "backend.h"

class MimeResolver {
public:
  MimeResolver(bool accurateMimeTypes) :
    m_accurateMimeTypes(accurateMimeTypes) {

  }

  ~MimeResolver() {

  }

  QString mimeTypeForFile(const QString& file) {
    return
      QMimeDatabase().mimeTypeForFile(file, m_accurateMimeTypes ? QMimeDatabase::MatchContent :
				      QMimeDatabase::MatchExtension).name();
  }

private:
  bool m_accurateMimeTypes;
};


DocumentsBrowserWorker::DocumentsBrowserWorker() :
  m_running(false),
  m_resolver(0) {

}

DocumentsBrowserWorker::~DocumentsBrowserWorker() {
  clearResolver();
}

void DocumentsBrowserWorker::start(bool indexSdCards, MimeResolver *resolver) {
  clearResolver();
  m_dirs.clear();

  m_resolver = resolver;
  m_running = true;

  m_dirs << QDir::homePath();

  if (indexSdCards) {
    m_dirs << "/media/sdcard/";
  }

  QThread::start();
}

void DocumentsBrowserWorker::stop() {
  m_running = false;
}

void DocumentsBrowserWorker::run() {
  for (const QString& dir : m_dirs) {
    scanDirectory(dir);
  }

  emit done();
}

void DocumentsBrowserWorker::scanDirectory(const QString& dir) {
  QStringList types(Backend::supportedMimeTypes());
  QDirIterator iter(dir, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);

  while (m_running && iter.hasNext()) {
    QString file = iter.next();

    QString mime = m_resolver->mimeTypeForFile(file);
    if (!mime.isEmpty() && types.indexOf(mime) != -1) {
      emit fileAvailable(file, mime);
    }
  }
}

void DocumentsBrowserWorker::clearResolver() {
  delete m_resolver;
  m_resolver = 0;
}

DocumentsBrowserModel::DocumentsBrowserModel(QObject *parent) :
  QAbstractListModel(parent) {

}

int DocumentsBrowserModel::rowCount(const QModelIndex& parent) const {
  return parent.isValid() ? 0 : m_info.size();
}

QVariant DocumentsBrowserModel::data(const QModelIndex& index, int role) const {
  if (index.row() < m_info.size()) {
    switch (role) {
    case NameRole:
      return QFileInfo(m_info[index.row()].first).completeBaseName();
    case PathRole:
      return m_info[index.row()].first;
    case MimeRole:
      return m_info[index.row()].second;
    default:
      break;
    }
  }

  return QVariant();
}

QHash<int, QByteArray> DocumentsBrowserModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[NameRole] = "name";
  roles[PathRole] = "path";
  roles[MimeRole] = "mime";

  return roles;
}

void DocumentsBrowserModel::clear() {
  if (m_info.size() > 0) {
    beginRemoveRows(QModelIndex(), 0, m_info.size() - 1);
    m_info.clear();
    endRemoveRows();
  }
}

void DocumentsBrowserModel::fileAvailable(const QString& file, const QString& mime) {
  beginInsertRows(QModelIndex(), m_info.size(), m_info.size());
  m_info.push_back(std::make_pair(file, mime));
  endInsertRows();
}

DocumentsBrowser::DocumentsBrowser(QObject *parent) :
  QObject(parent),
  m_worker(0),
  m_model(new DocumentsBrowserModel(this)) {

}

DocumentsBrowser::~DocumentsBrowser() {
  stopWorker();
}

QObject *DocumentsBrowser::model() {
  return m_model;
}

bool DocumentsBrowser::isBusy() const {
  return m_worker != 0;
}

void DocumentsBrowser::refresh(bool accurateMimeTypes, bool indexSdCards) {
  stopWorker();

  m_model->clear();

  MimeResolver *resolver = new MimeResolver(accurateMimeTypes);

  DocumentsBrowserWorker *worker = new DocumentsBrowserWorker;
  QObject::connect(worker, SIGNAL(fileAvailable(const QString&, const QString&)),
		   m_model, SLOT(fileAvailable(const QString&, const QString&)),
		   Qt::QueuedConnection);
  QObject::connect(worker, SIGNAL(done()), this, SLOT(stopWorker()), Qt::QueuedConnection);
  worker->start(indexSdCards, resolver);

  m_worker = worker;

  emit busyChanged();
}

void DocumentsBrowser::stopWorker() {
  if (m_worker && m_worker->isRunning()) {
    QObject::disconnect(m_worker, SIGNAL(fileAvailable(const QString&, const QString&)),
			m_model, SLOT(fileAvailable(const QString&, const QString&)));
    QObject::disconnect(m_worker, SIGNAL(done()), this, SLOT(stopWorker()));
    m_worker->stop();
    m_worker->wait();
  }

  delete m_worker;
  m_worker = 0;

  emit busyChanged();
}
