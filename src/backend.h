#ifndef BACKEND_H
#define BACKEND_H

#include <QSizeF>
#include <QImage>
#include <QRectF>

class BackendPage {
public:
  virtual QSizeF size() = 0;
  virtual QImage render(qreal dpiX, qreal dpiY, const QRectF& rect) = 0;

  virtual ~BackendPage() {}

protected:
  BackendPage() {}
};

class Backend {
public:
  static Backend *create(const QString& filePath);

  virtual int numPages() = 0;
  virtual BackendPage *page(int num) = 0;

  virtual ~Backend() {}

protected:
  Backend() {}
  virtual bool load(const QString& filePath) = 0;
};

#endif /* BACKEND_H */
