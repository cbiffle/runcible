#include "tocman.h"
#include <poppler-qt4.h>

TocManager::TocManager(const QString &path, QObject *parent)
    : QObject(parent),
      _path(path),
      _coordinator(path, this) { }

TocManager::~TocManager() { }

static void copyToc(const QUrl &docUrl, Poppler::Document *doc, QDomNode node,
    QList<Choice> &choices, int indent) {
  while (!node.isNull()) {
    if (node.isElement()) {
      QDomElement el = node.toElement();
      QString destName = el.attribute("DestinationName", QString());
      if (!destName.isNull()) {
        Poppler::LinkDestination *dest = doc->linkDestination(destName);
        QUrl refUrl(docUrl);
        refUrl.setFragment("page:" + QString::number(dest->pageNumber() - 1));
        delete dest;
       
        QString displayName(QString(indent * 2, ' ') + el.nodeName());
        choices << Choice(displayName, refUrl.toString()); 
      }
    }
    if (node.hasChildNodes()) {
      copyToc(docUrl, doc, node.firstChild(), choices, indent + 1);
    }
    node = node.nextSibling();
  }
}

void TocManager::refresh() {
  Poppler::Document *doc = Poppler::Document::load(_path);

  if (doc) {
    QList<Choice> choices;
    QUrl docUrl("file:" + _path);
    QDomDocument *toc = doc->toc();
    if (toc) {
      QDomNode child = toc->documentElement();
      copyToc(docUrl, doc, child, choices, 0);
      /*
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
      */
      delete toc;
    } else {
      qDebug() << "NO TOC";
    }

    emit contentsChanged(choices);
  }

  delete doc;
}

void TocManager::activate(Choice c) {
  qDebug() << c.id();
  _coordinator.openItem("runcible-view-pdf", QUrl(c.id()));
}
