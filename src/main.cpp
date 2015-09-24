#include <QGuiApplication>
#include <QQuickView>
#include <QScopedPointer>
#include <QDebug>
#include <QQmlError>
#include <MDeclarativeCache>

#include "document.h"
#include "document-view.h"
#include "documents-browser.h"
#include "document-settings.h"
#include "icon-provider.h"

#define URI    "Documents"
#define MAJOR  1
#define MINOR  0

Q_DECL_EXPORT int
main(int argc, char *argv[]) {
  QScopedPointer<QGuiApplication> app(MDeclarativeCache::qApplication(argc, argv));
  QScopedPointer<QQuickView> view(MDeclarativeCache::qQuickView());

  app->setApplicationName("harbour-documents");
  app->setApplicationDisplayName(QObject::tr("Document viewer"));
  view->setTitle(app->applicationDisplayName());

  view->setResizeMode(QQuickView::SizeRootObjectToView);

  view->engine()->addImageProvider(QLatin1String("svg"), new IconProvider);

  qmlRegisterType<Document>(URI, MAJOR, MINOR, "Document");
  qmlRegisterType<DocumentView>(URI, MAJOR, MINOR, "DocumentView");
  qmlRegisterType<DocumentsBrowser>(URI, MAJOR, MINOR, "DocumentsBrowser");
  qmlRegisterType<DocumentSettings>(URI, MAJOR, MINOR, "DocumentSettings");

  view->setSource(QUrl("qrc:/qml/main.qml"));

  if (view->status() == QQuickView::Error) {
    qCritical() << "Errors loading QML:";
    QList<QQmlError> errors = view->errors();

    foreach (const QQmlError& error, errors) {
      qCritical() << error.toString();
    }

    return 1;
  }

  //  view->showFullScreen();
  view->show();

  return app->exec();
}
