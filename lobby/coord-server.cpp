#include "coord-server.h"

CoordinatorServer::CoordinatorServer(QObject *parent)
    : QObject(parent),
      _c("runcible/coordinator", this) {
  connect(&_c, SIGNAL(received(QString, QByteArray)), this, SLOT(received(QString, QByteArray)));
}

CoordinatorServer::~CoordinatorServer() { }

void CoordinatorServer::received(const QString &message, const QByteArray &data) {
  QDataStream in(data);
  if (message == "openItem(QUrl)") {
    QUrl url;
    in >> url;
    openItem(url);
  } else if (message == "openItem(QString,QUrl)") {
    QString program;
    QUrl url;
    in >> program >> url;
    openItem(program, url);
  } else {
    qDebug() << "Coordinator received unrecognized message" << message;
  }
}

void CoordinatorServer::openItem(QUrl item) {
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

void CoordinatorServer::openItem(QString program, QUrl item) {
  QProcess::startDetached(program, QStringList() << item.toString());
}
