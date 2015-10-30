#include "outline-model.h"
#include "backend.h"
#include "document.h"
#include <QDebug>

OutlineModel::OutlineModel(QObject *parent) :
  QAbstractListModel(parent), m_doc(nullptr) {
}

void OutlineModel::reset() {
  m_doc = nullptr;
  m_outline.reset();
}

Document *OutlineModel::document() const {
  return m_doc;
}

void OutlineModel::setDocument(Document *document) {
  if (m_doc != document) {

    if (m_doc) {
      QObject::disconnect(m_doc, SIGNAL(aboutToReset()), this, SLOT(reset()));
    }

    m_doc = document;

    if (m_doc) {
      QObject::connect(m_doc, SIGNAL(aboutToReset()), this, SLOT(reset()));
    }
    m_outline = document->outline();
    emit documentChanged();
  }
}

int OutlineModel::rowCount(const QModelIndex& parent) const {
  return parent.isValid() && m_outline ? 0 : m_outline->size();
}

QHash<int, QByteArray> OutlineModel::roleNames() const
{
  QHash<int, QByteArray> roles = {
    {LevelRole, "level"}, {TitleRole, "title"}, {PageRole, "page"}};
  return roles;
}

QVariant OutlineModel::data(const QModelIndex& index, int role) const {
  auto row = index.row();
  QVariant res;
  if (m_outline && row < m_outline->size()) {
    switch (role) {
    case LevelRole:
      res = m_outline->level(row);
      break;
    case TitleRole:
    case Qt::DisplayRole:
      res = m_outline->title(row);
      break;
    case PageRole:
      res = m_outline->page(row);
      break;
    default:
      break;
    }
  }
  return res;
}
