#ifndef LO_PACKEND_H
#define LO_BACKEND_H

#include "backend.h"

namespace lok {
  class Office;
  class Document;
};

class LoBackend : public Backend {
public:
  LoBackend();
  ~LoBackend();
  int numPages();
  BackendPage *page(int num);
  bool load(const QString& filePath);
  void reset();
  bool isLocked();
  bool unlock(const QString& password);

private:
  lok::Document *m_doc;
  static lok::Office *m_office;
};

#endif /* MUPDF_BACKEND_H */
