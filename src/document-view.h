#ifndef DOCUMENT_VIEW_H
#define DOCUMENT_VIEW_H

#include <QQuickPaintedItem>
#include <QTimer>
#include <QThread>
#include "tile-cache.h"

class Document;
class DocumentPage;

class DocumentView : public QQuickPaintedItem {
  Q_OBJECT
  Q_PROPERTY(Document *document READ document WRITE setDocument NOTIFY documentChanged);
  Q_PROPERTY(qreal contentX READ contentX WRITE setContentX NOTIFY contentXChanged);
  Q_PROPERTY(qreal contentY READ contentY WRITE setContentY NOTIFY contentYChanged);

public:
  DocumentView(QQuickItem *parent = 0);
  ~DocumentView();

  Document *document() const;
  void setDocument(Document *document);

  qreal contentX() const;
  void setContentX(qreal x);

  qreal contentY() const;
  void setContentY(qreal y);

  void paint(QPainter *painter);

protected:
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

signals:
  void documentChanged();
  void contentXChanged();
  void contentYChanged();

private slots:
  void refreshTiles();
  void tileAvailable(const Tile& tile, qint64 cookie);
  void init();

private:
  void deleteCache();
  void createCache();
  QRectF tileRect(const Tile& tile);
  QList<QRectF> pageRectangles(DocumentPage *page);

  Document *m_doc;
  TileCache *m_cache;
  qreal m_x;
  qreal m_y;
  QTimer m_timer;
  qint64 m_cookie;
  QList<Tile> m_tiles;
};

#endif /* DOCUMENT_VIEW_H */
