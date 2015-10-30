#ifndef DOCUMENTS_FWD_H
#define DOCUMENTS_FWD_H

#include <memory>

class DocumentPage;
class Backend;
class DocumentLoader;
class Document;

class BackendOutline;
typedef std::unique_ptr<BackendOutline> BackendOutlineHandle;

#endif /* DOCUMENTS_FWD_H */
