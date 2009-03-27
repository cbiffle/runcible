#include "dirman.h"

#include <QApplication>

DirManager::DirManager(const QString &path, QObject *parent)
    : QObject(parent),
      _basePath(path) {
}

DirManager::~DirManager() {}

void DirManager::activate(const Choice choice) {
  QString path = choice.id();
  qDebug() << path;
  QFileInfo fileInfo(path);
  if (fileInfo.isDir()) {
    QProcess::startDetached(QApplication::applicationFilePath(), QStringList() << path);
  } else {
    QString viewer = "runcible-view-" + fileInfo.suffix();
    if (!QProcess::startDetached(viewer, QStringList() << path)) {
      QApplication::beep();
      qDebug() << "Viewer not found.";
    }
  }
}

void DirManager::refresh() {
  QDirIterator it(_basePath);
  QList<Choice> choices;
  while (it.hasNext()) {
    it.next();
    if (it.fileName() != "." && it.fileName() != "..") {
      choices << Choice(it.fileName(), it.filePath());
    }
  }

  emit contentsChanged(choices);
}
