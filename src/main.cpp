#include <QGuiApplication>
#include <QQuickView>
#include <QScopedPointer>
#include <QDebug>
#include <QQmlError>

#include "document.h"
#include "document-view.h"

int main(int argc, char *argv[]) {
  QScopedPointer<QGuiApplication> app(new QGuiApplication(argc, argv));
  QScopedPointer<QQuickView> view(new QQuickView);

  app->setApplicationName("harbour-documents");

  view->setResizeMode(QQuickView::SizeRootObjectToView);

  qmlRegisterType<Document>("Poppler", 1, 0, "Document");
  qmlRegisterType<DocumentView>("Poppler", 1, 0, "DocumentView");

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
