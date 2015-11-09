#ifndef POPPLER_PACKEND_H
#define POPPLER_BACKEND_H

#include "backend.h"

typedef struct fz_document_s fz_document;
typedef struct fz_context_s fz_context;

class MupdfBackend : public Backend {
public:
  MupdfBackend();
  ~MupdfBackend();
  int numPages();
  BackendPage *page(int num);
  bool load(const QString& filePath);
  bool isLocked();
  bool unlock(const QString& password);
  BackendOutlineHandle outline() override;

private:
  fz_document *m_doc;
  fz_context *m_ctx;
};

#endif /* MUPDF_BACKEND_H */
