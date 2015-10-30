#ifndef OUTLINE_MODEL_H
#define OUTLINE_MODEL_H

#include <QAbstractListModel>
#include "documents-fwd.h"
#include "backend.h"

class OutlineModel : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(Document *document READ document WRITE setDocument NOTIFY documentChanged);

public:
  OutlineModel(QObject *parent = 0);
  ~OutlineModel() {}

  enum {
    LevelRole = Qt::UserRole + 1,
    TitleRole,
    PageRole
  };

  int rowCount(const QModelIndex&) const override;
  QVariant data(const QModelIndex&, int) const override;

  Document *document() const;
  void setDocument(Document *document);

  QHash<int, QByteArray> roleNames() const;

signals:
  void documentChanged();

private slots:
  void reset();

private:
  Document *m_doc;
  BackendOutlineHandle m_outline;
};

#endif /* OUTLINE_MODEL_H */
