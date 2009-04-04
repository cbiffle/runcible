#include "dirman.h"

#include <QApplication>

DirManager::DirManager(const QString &path, QObject *parent)
    : QObject(parent),
      _basePath(path),
      _coordinator("dir-list", this) {
}

DirManager::~DirManager() {}

void DirManager::activate(const Choice choice) {
  QUrl item(choice.id());
  _coordinator.openItem(item);
}

void DirManager::refresh() {
  QDirIterator it(_basePath);
  QList<Choice> choices;
  while (it.hasNext()) {
    it.next();
    if (it.fileName() != "." && it.fileName() != "..") {
      QString displayName(it.fileName());
      if (it.fileInfo().isDir()) {
        displayName += "/";
      }
      choices << Choice(displayName, "file:" + it.filePath());
    }
  }

  qSort(choices);

  emit contentsChanged(choices);
}
