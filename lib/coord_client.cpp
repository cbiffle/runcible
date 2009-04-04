#include "coord_client.h"

#define C_COORD "runcible/coordinator"
#define C_FOOT "runcible/footer"

#include <QCopChannel>

CoordinatorClient::CoordinatorClient(const QString &, QObject *parent)
    : QObject(parent) { }

CoordinatorClient::~CoordinatorClient() { }

void CoordinatorClient::openItem(QUrl item) {
  QByteArray data;
  QDataStream out(&data, QIODevice::WriteOnly);
  out << item;
  qDebug() << "Sending openItem(QUrl)";
  if (!QCopChannel::send(C_COORD, "openItem(QUrl)", data)) {
    qDebug() << "Send failed.";
  }
}

void CoordinatorClient::openItem(QString program, QUrl item) {
  QByteArray data;
  QDataStream out(&data, QIODevice::WriteOnly);
  out << program << item;
  qDebug() << "Sending openItem(QString,QUrl)";
  if (!QCopChannel::send(C_COORD, "openItem(QString,QUrl)", data)) {
    qDebug() << "Send failed.";
  }
}
