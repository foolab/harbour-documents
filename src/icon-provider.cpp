#include "icon-provider.h"
#include <QImageReader>
#include <QColor>
#include <QPainter>

IconProvider::IconProvider() :
  QQuickImageProvider(QQuickImageProvider::Image) {

}

QImage IconProvider::requestImage(const QString& id, QSize *size, const QSize& requestedSize) {
  QStringList parts = id.split('?');

  if (parts.isEmpty()) {
    return QImage();
  }

  QSize srcSize = requestedSize.isValid() ? requestedSize : QSize(48, 48);

  QImageReader reader(QString(":/icons/%1").arg(parts[0]));
  reader.setScaledSize(srcSize);
  QImage img = reader.read();
  if (size) {
    *size = srcSize;
  }

  if (parts.size() > 1 && QColor::isValidColor(parts[1])) {
    QPainter painter(&img);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(QRect(QPoint(0, 0), img.size()), parts.at(1));
    painter.end();
  }

  return img;
}

