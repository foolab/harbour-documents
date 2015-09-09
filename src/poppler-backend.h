#ifndef POPPLER_PACKEND_H
#define POPPLER_BACKEND_H

#include "backend.h"

namespace Poppler {
  class Document;
};

class PopplerBackend : public Backend {
public:
  PopplerBackend();
  ~PopplerBackend();
  int numPages();
  BackendPage *page(int num);
  bool load(const QString& filePath);
  void reset();

private:
  Poppler::Document *m_doc;
};

#endif /* POPPLER_BACKEND_H */
