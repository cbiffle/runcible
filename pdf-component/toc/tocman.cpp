#include "tocman.h"
#include <poppler-qt4.h>

TocManager::TocManager(const QString &path, QObject *parent)
    : QObject(parent),
      _path(path),
      _coordinator(path, this) { }

TocManager::~TocManager() { }

void TocManager::refresh() {
  Poppler::Document *doc = Poppler::Document::load(_path);

  if (doc) {
    QList<Choice> choices;
    QDomDocument *toc = doc->toc();
    if (toc) {
      QDomNode child = toc->documentElement();
      while (!child.isNull()) {
        if (child.isElement()) {
          QDomElement el = child.toElement();
          QString destName = el.attribute("DestinationName", "");
          Poppler::LinkDestination *dest = doc->linkDestination(destName);
          QUrl url("file:" + _path + "#page:" + QString::number(dest->pageNumber()));
          choices << Choice(el.nodeName(), url.toString());
          delete dest;
        }
        child = child.nextSibling();
      }
      delete toc;
    } else {
      qDebug() << "NO TOC";
    }

    emit contentsChanged(choices);
  }

  delete doc;
}

void TocManager::activate(Choice c) {
  _coordinator.openItem("runcible-view-pdf", QUrl(c.id()));
}
