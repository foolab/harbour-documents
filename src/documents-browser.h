#ifndef DOCUMENTS_BROWSER_H
#define DOCUMENTS_BROWSER_H

#include <QObject>

class QAbstractItemModel;

class DocumentsBrowser : public QObject {
  Q_OBJECT
  Q_PROPERTY(QObject* model READ model NOTIFY modelChanged);

public:
  DocumentsBrowser(QObject *parent = 0);
  ~DocumentsBrowser();

  QObject *model();

signals:
  void modelChanged();

private:
  QAbstractItemModel *m_model;
};

#endif /* DOCUMENTS_BROWSER_H */
