#include "backend.h"
#include "poppler-backend.h"
#include "mupdf-backend.h"

Backend *Backend::create(const QString& filePath) {
  Backend *backend = new MupdfBackend;

  if (backend->load(filePath)) {
    return backend;
  }

  delete backend;
  return 0;
}
