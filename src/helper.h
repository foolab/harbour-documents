#ifndef HELPER_H
#define HELPER_H

#include <QObject>

class QQmlEngine;
class QJSEngine;

class Helper : public QObject {
  Q_OBJECT

public:
  Helper(QObject *parent = 0);
  ~Helper();

  Q_INVOKABLE QString fileName(const QString& path);
  Q_INVOKABLE QString mimeDescription(const QString& mimeType);
};

QObject *helper_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine);

#endif /* HELPER_H */
