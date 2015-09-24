#include "document-settings.h"
#include <QFileInfo>
#include <QSettings>
#include <QDebug>
#include <QStandardPaths>
#include <QCryptographicHash>

DocumentSettings::DocumentSettings(QObject *parent) :
  QObject(parent),
  m_settings(0) {

}

DocumentSettings::~DocumentSettings() {
  save();

  if (m_settings) {
    delete m_settings;
    m_settings = 0;
  }
}

qreal DocumentSettings::zoom() const {
  return m_settings ? m_settings->value("info/zoom", 0).toReal() : 0;
}

void DocumentSettings::setZoom(qreal zoom) {
  if (m_settings && DocumentSettings::zoom() != zoom) {
    m_settings->setValue("info/zoom", zoom);
    emit zoomChanged();
  }
}

QPointF DocumentSettings::position() const {
  return m_settings ? m_settings->value("info/position", QPointF(0, 0)).value<QPointF>() : QPointF(0, 0);
}

void DocumentSettings::setPosition(const QPointF& position) {
  if (m_settings && DocumentSettings::position() != position) {
    m_settings->setValue("info/position", position);
    emit positionChanged();
  }
}

void DocumentSettings::load(const QString& filePath) {
  QString path(QFileInfo(filePath).canonicalFilePath());

  if (m_settings) {
    delete m_settings;
  }

  QCryptographicHash md5(QCryptographicHash::Md5);
  md5.addData(path.toUtf8());

  QString ini =
    QString("%1/info/%2.info")
    .arg(QStandardPaths::writableLocation(QStandardPaths::CacheLocation))
    .arg(QString::fromLatin1(md5.result().toHex()));

  m_settings = new QSettings(ini, QSettings::IniFormat, this);

  m_settings->setValue("info/filePath", path);
  m_settings->setValue("info/version", 1);

  emit zoomChanged();
  emit positionChanged();
}

void DocumentSettings::save() {
  if (m_settings) {
    m_settings->sync();
  }
}
