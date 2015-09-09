#include "mupdf-backend.h"
extern "C" {
#include "mupdf/fitz.h"
};

// TODO: we are not handling any exceptions at all
// TODO: locking

class MupdfPage : public BackendPage {
public:
  MupdfPage(fz_page *page, fz_context *ctx) :
    m_page(page),
    m_ctx(ctx),
    m_list(0),
    m_dev(0) {}

  ~MupdfPage() {
    if (m_list) {
      fz_drop_display_list(m_ctx, m_list);
      m_list = 0;
    }

    if (m_dev) {
      fz_drop_device(m_ctx, m_dev);
      m_dev = 0;
    }

    fz_drop_page(m_ctx, m_page);
    m_page = 0;
    m_ctx = 0;
  }

  QSizeF size() {
    fz_rect rect;
    fz_bound_page(m_ctx, m_page, &rect);

    return QSizeF(rect.x1 - rect.x0, rect.y1 - rect.y0);
  }

  QImage render(qreal dpiX, qreal dpiY, const QRectF& rect) {
    fz_matrix matrix;
    fz_scale(&matrix, dpiX / 72.0f, dpiY / 72.0f);

    fz_rect r;
    fz_bound_page(m_ctx, m_page, &r);
    fz_transform_rect(&r, &matrix);

    fz_irect irect;
    fz_round_rect(&irect, &r);

    // TODO: reset this when zoom changes (after we cache the run result).
    if (!m_list) {
      m_list = fz_new_display_list(m_ctx);
    }

    // TODO: same as above
    if (!m_dev) {
      m_dev = fz_new_list_device(m_ctx, m_list);
    }

    // TODO: cache this and reset it when zoom changes
    fz_run_page(m_ctx, m_page, m_dev, &matrix, 0);

    fz_matrix tile;
    fz_translate(&tile, -r.x0, -r.y0);

    fz_pre_translate(&tile, -rect.x(), -rect.y());

    fz_rect tr = fz_infinite_rect;
    tr.x0 = tr.y0 = 0.0;
    int width = tr.x1 = rect.width();
    int height = tr.y1 = rect.height();

    QImage image(width, height, QImage::Format_RGB32);
    image.fill(Qt::white); // TODO: configurable

    fz_pixmap* pixmap =
      fz_new_pixmap_with_data(m_ctx,
			      fz_device_bgr(m_ctx),
			      image.width(),
			      image.height(),
			      image.bits());

    fz_device *device = fz_new_draw_device(m_ctx, pixmap);
    fz_run_display_list(m_ctx, m_list, device, &tile, &tr, 0);
    fz_drop_device(m_ctx, device);
    fz_drop_pixmap(m_ctx, pixmap);
    return image;
  }

private:
  fz_page *m_page;
  fz_context *m_ctx;
  fz_display_list *m_list;
  fz_device *m_dev;
};

MupdfBackend::MupdfBackend() :
  m_doc(0),
  m_ctx(0) {

}

MupdfBackend::~MupdfBackend() {
  if (m_doc) {
    fz_drop_document(m_ctx, m_doc);
    m_doc = 0;
  }

  if (m_ctx) {
    fz_drop_document_handler_context(m_ctx); // TODO: is this needed?
    fz_drop_context(m_ctx);
    m_ctx = 0;
  }
}

int MupdfBackend::numPages() {
  return m_doc ? fz_count_pages(m_ctx, m_doc) : 0;
}

BackendPage *MupdfBackend::page(int num) {
  fz_page *p = fz_load_page(m_ctx, m_doc, num);
  return new MupdfPage(p, m_ctx);
}

bool MupdfBackend::load(const QString& filePath) {
  Q_ASSERT(m_doc == 0);
  Q_ASSERT(m_ctx == 0);

  m_ctx = fz_new_context(NULL, NULL, FZ_STORE_DEFAULT); // TODO: locks
  if (!m_ctx) {
    return false;
  }

  fz_register_document_handlers(m_ctx);

  m_doc = fz_open_document(m_ctx, filePath.toUtf8());
  if (!m_doc) {
    fz_drop_context(m_ctx);
    m_ctx = 0;
  }

  return true;
}

