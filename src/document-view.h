#ifndef DOCUMENT_VIEW_H
#define DOCUMENT_VIEW_H

#include <QQuickItem>
#include <QTimer>
#include "tile-cache.h"

class Document;
class DocumentPage;
class Renderer;

class DocumentView : public QQuickItem {
  Q_OBJECT
  Q_PROPERTY(Document *document READ document WRITE setDocument NOTIFY documentChanged);
  Q_PROPERTY(qreal contentX READ contentX WRITE setContentX NOTIFY contentXChanged);
  Q_PROPERTY(qreal contentY READ contentY WRITE setContentY NOTIFY contentYChanged);
  Q_PROPERTY(qreal dpiX READ dpiX NOTIFY dpiChanged);
  Q_PROPERTY(qreal dpiY READ dpiY NOTIFY dpiChanged);
  Q_PROPERTY(qreal zoom READ zoom WRITE setZoom NOTIFY zoomChanged);

public:
  DocumentView(QQuickItem *parent = 0);
  ~DocumentView();

  Document *document() const;
  void setDocument(Document *document);

  qreal contentX() const;
  void setContentX(qreal x);

  qreal contentY() const;
  void setContentY(qreal y);

  qreal zoom() const;
  void setZoom(qreal zoom);

  qreal dpiX() const;
  qreal dpiY() const;

  QRectF tileRect(const Tile& tile);

protected:
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);
  QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *updatePaintNodeData);

signals:
  void documentChanged();
  void contentXChanged();
  void contentYChanged();
  void dpiChanged();
  void zoomChanged();

private slots:
  void refreshTiles();
  void tileAvailable(const Tile& tile, qint64 cookie);
  void tileRequestDone(qint64 cookie);
  void init();

private:
  void deleteCache();
  void createCache();
  QList<QRectF> pageRectangles(DocumentPage *page);
  void resetZoom();

  Renderer *m_renderer;
  Document *m_doc;
  TileCache *m_cache;
  qreal m_x;
  qreal m_y;
  qreal m_zoom;
  qreal m_dpiX;
  qreal m_dpiY;
  QTimer m_timer;
};

#endif /* DOCUMENT_VIEW_H */
