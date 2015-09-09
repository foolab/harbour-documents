#include "backend.h"
#include "poppler-backend.h"

Backend *Backend::create(const QString& filePath) {
  Backend *backend = new PopplerBackend;
  if (backend->load(filePath)) {
    return backend;
  }

  delete backend;
  return 0;
}
