#ifndef DOCUMENT_VIEW_H
#define DOCUMENT_VIEW_H

#include <QQuickPaintedItem>
#include <QTimer>
#include "tile-cache.h"

class PopplerDocument;

class DocumentView : public QQuickPaintedItem {
  Q_OBJECT
  Q_PROPERTY(PopplerDocument *document READ document WRITE setDocument NOTIFY documentChanged);
  Q_PROPERTY(qreal contentX READ contentX WRITE setContentX NOTIFY contentXChanged);
  Q_PROPERTY(qreal contentY READ contentY WRITE setContentY NOTIFY contentYChanged);

public:
  DocumentView(QQuickItem *parent = 0);
  ~DocumentView();

  PopplerDocument *document() const;
  void setDocument(PopplerDocument *document);

  qreal contentX() const;
  void setContentX(qreal x);

  qreal contentY() const;
  void setContentY(qreal y);

  void paint(QPainter *painter);

signals:
  void documentChanged();
  void contentXChanged();
  void contentYChanged();

private slots:
  void refreshTiles();
  void tileAvailable(Tile tile);

private:
  PopplerDocument *m_doc;
  TileCache *m_cache;
  qreal m_x;
  qreal m_y;
  QTimer m_timer;
  QList<Tile> m_tiles;
};

#endif /* DOCUMENT_VIEW_H */
