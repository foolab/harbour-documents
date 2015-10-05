#ifndef BACKEND_H
#define BACKEND_H

#include <QSizeF>
#include <QImage>
#include <QRectF>
#include <QStringList>
#include <functional>

class Backend;

class BackendInfo {
public:
BackendInfo(const QString& mime, int score) :
  m_mime(mime),
  m_score(score) {
  }

  QString m_mime;
  int m_score;
};

class BackendPage {
public:
  virtual QSizeF size() = 0;
  virtual QImage render(qreal dpiX, qreal dpiY, const QRectF& rect) = 0;

  virtual ~BackendPage() {}

  virtual void reset() = 0;

  virtual qreal toPixels(qreal value) const = 0;

protected:
  BackendPage() {}
};

class Backend {
public:
  static Backend *create(const QString& filePath, const QString& mimeType);

  virtual ~Backend() {}

  virtual int numPages() = 0;
  virtual BackendPage *page(int num) = 0;

  virtual bool isLocked() = 0;
  virtual bool unlock(const QString& password) = 0;

  // This must return int or g++ will barf :/
  static int registerBackend(const QList<BackendInfo>& info,
			     const std::function<Backend *(void)>& func);
  static QStringList supportedMimeTypes();

protected:
  Backend() {}
  virtual bool load(const QString& filePath) = 0;
};

// we must assign the function return value otherwise g++ will barf
#define ADD_BACKEND(info,klass)						\
  static int foo =							\
    Backend::registerBackend(info, []() -> Backend *{ return new klass; });

/*
  #define ADD_BACKEND(ext,score,klass)					\
  namespace {								\
  static std::function<Backend *(void)>					\
  klass_func([]() -> Backend *{ return new klass; });			\
  static int foo = Backend::registerBackend(ext, score, klass_func);	\
  };
*/

#endif /* BACKEND_H */
