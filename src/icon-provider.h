#ifndef ICON_PROVIDER_H
#define ICON_PROVIDER_H

#include <QQuickImageProvider>

class IconProvider : public QQuickImageProvider {
public:
  IconProvider();
  QImage requestImage(const QString& id, QSize *size, const QSize& requestedSize);
};

#endif /* ICON_PROVIDER_H */
