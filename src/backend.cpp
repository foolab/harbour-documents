#include "backend.h"
#include <QFileInfo>
#include <map>
#include <list>

static std::map<QString, std::map<int, std::list<std::function<Backend*(void)> > > > backends;

Backend *Backend::create(const QString& filePath) {
  QString ext(QFileInfo(filePath).completeSuffix().toLower());
  if (ext.isEmpty()) {
    return 0;
  }

  if (!ext.startsWith('.')){
    ext.prepend('.');
  }

  std::map<QString, std::map<int, std::list<std::function<Backend*(void)> > > >::iterator iter1 =
    backends.find(ext);
  if (iter1 == backends.end()) {
    return 0;
  }

  std::map<int, std::list<std::function<Backend*(void)> > >::const_reverse_iterator iter2;
  for (iter2 = iter1->second.crbegin(); iter2 != iter1->second.crend(); iter2++) {
    // for each scrore we check all backends:
    for (std::list<std::function<Backend*(void)> >::const_iterator iter =
	   iter2->second.cbegin(); iter != iter2->second.cend(); iter++) {
      const std::function<Backend*()> func(*iter);
      Backend *b = func();

      if (b->load(filePath)) {
	return b;
      }

      delete b;
    }
  }

  return 0;
}

int Backend::registerBackend(const QString& ext, const QString& mime, int score,
			     const std::function<Backend *(void)>& func) {
  backends[ext][score].push_back(func);

  return 0;
}
