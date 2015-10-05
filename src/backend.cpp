#include "backend.h"
#include <map>
#include <list>

static std::map<QString, std::map<int, std::list<std::function<Backend*(void)> > > > backends;

QStringList Backend::supportedMimeTypes() {
  QStringList types;

  std::map<QString, std::map<int, std::list<std::function<Backend*(void)> > > >::const_iterator iter;
  for (iter = backends.cbegin(); iter != backends.cend(); iter++) {
    if (types.indexOf(iter->first) == -1) {
      types << iter->first;
    }
  }

  return types;
}

Backend *Backend::create(const QString& filePath, const QString& mimeType) {
  if (mimeType.isEmpty()) {
    return 0;
  }

  std::map<QString, std::map<int, std::list<std::function<Backend*(void)> > > >::iterator iter1 =
    backends.find(mimeType);
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

int Backend::registerBackend(const QList<BackendInfo>& info,
			     const std::function<Backend *(void)>& func) {

  foreach (const BackendInfo& i, info) {
    backends[i.m_mime][i.m_score].push_back(func);
  }

  return 0;
}
