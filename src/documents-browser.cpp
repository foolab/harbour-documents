#include "documents-browser.h"
#include <QAbstractListModel>
#include <QFileInfo>
#include <QDirIterator>
#include <QDir>
#include "backend.h"

// TODO: this is UGLY
class FilesModel : public QAbstractListModel {
public:
  enum {
    NameRole = Qt::UserRole + 1,
    PathRole = Qt::UserRole + 2,
  };

  FilesModel(QObject *parent = 0) :
    QAbstractListModel(parent) {
    QDirIterator iter(QDir::home(), QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
    QStringList exts(Backend::supportedExtensions());

    while (iter.hasNext()) {
      QString file = iter.next();
      QString ext(QFileInfo(file).suffix().toLower());

      if (ext.isEmpty()) {
	continue;
      }

      if (!ext.startsWith('.')){
	ext.prepend('.');
      }

      if (exts.indexOf(ext) != -1) {
	m_files << file;
      }
    }
  }

  int rowCount(const QModelIndex& parent = QModelIndex()) const {
    return parent.isValid() ? 0 : m_files.size();
  }

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const {
    if (index.row() < m_files.size()) {
      switch (role) {
      case NameRole:
	return QFileInfo(m_files[index.row()]).fileName();
      case PathRole:
	return m_files[index.row()];
      default:
	break;
      }
    }

    return QVariant();
  }

  QHash<int, QByteArray> roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[PathRole] = "path";

    return roles;
  }

private:
  QStringList m_files;
};

DocumentsBrowser::DocumentsBrowser(QObject *parent) :
  QObject(parent),
  m_model(0) {

}

DocumentsBrowser::~DocumentsBrowser() {

}

QAbstractItemModel *DocumentsBrowser::model() {
  if (!m_model) {
    m_model = new FilesModel(this);
  }

  return m_model;
}
