#ifndef DOCUMENT_SETTINGS_H
#define DOCUMENT_SETTINGS_H

#include <QObject>
#include <QPointF>

class QSettings;

class DocumentSettings : public QObject {
  Q_OBJECT
  Q_PROPERTY(qreal zoom READ zoom WRITE setZoom NOTIFY zoomChanged);
  Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged);

public:
  DocumentSettings(QObject *parent = 0);
  ~DocumentSettings();

  qreal zoom() const;
  void setZoom(qreal zoom);

  QPointF position() const;
  void setPosition(const QPointF& position);

public slots:
  void load(const QString& filePath);
  void save();

signals:
  void zoomChanged();
  void positionChanged();

private:
  QSettings *m_settings;
};


#endif /* DOCUMENT_SETTINGS_H */
