#include "helper.h"
#include <QFileInfo>
#include <QMimeDatabase>
#include <QMimeType>

Helper::Helper(QObject *parent) :
  QObject(parent) {

}

Helper::~Helper() {

}

QString Helper::fileName(const QString& path) {
  return QFileInfo(path).fileName();
}

QString Helper::mimeDescription(const QString& mimeType) {
  return QMimeDatabase().mimeTypeForName(mimeType).comment();
}

QObject *helper_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine) {
  Q_UNUSED(engine);
  Q_UNUSED(scriptEngine);

  return new Helper;
}
