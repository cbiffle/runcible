#include "coord_client.h"

CoordinatorClient::CoordinatorClient(const QString &, QObject *parent)
    : QObject(parent) { }

CoordinatorClient::~CoordinatorClient() { }

void CoordinatorClient::openItem(QUrl item) {
  // TODO(cbiffle): this is stubbed out to avoid using COP.
  
  if (item.scheme() == "file") {
    QFileInfo fileInfo(item.path());
    QString binary;
    if (fileInfo.isDir()) {
      binary = "runcible-dir-list";
    } else {
      binary = "runcible-open-ext-" + fileInfo.suffix();
    }
    QProcess::startDetached(binary, QStringList() << fileInfo.absoluteFilePath());
  } else {
    qDebug() << "Cannot open non-file item " << item;
  }
}
