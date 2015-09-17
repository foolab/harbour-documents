#include "backend.h"
#include <QFileInfo>
#include <map>
#include <list>

static std::map<QString, std::map<int, std::list<std::function<Backend*(void)> > > > backends;

QStringList Backend::supportedExtensions() {
  QStringList ext;

  std::map<QString, std::map<int, std::list<std::function<Backend*(void)> > > >::const_iterator iter;
  for (iter = backends.cbegin(); iter != backends.cend(); iter++) {
    if (ext.indexOf(iter->first) == -1) {
      ext << iter->first;
    }
  }

  return ext;
}

Backend *Backend::create(const QString& filePath) {
  QString ext(QFileInfo(filePath).suffix().toLower());
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

int Backend::registerBackend(const QList<BackendInfo>& info,
			     const std::function<Backend *(void)>& func) {

  foreach (const BackendInfo& i, info) {
    backends[i.m_ext][i.m_score].push_back(func);
  }

  return 0;
}
