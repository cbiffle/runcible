#include "rwindow.h"

#include <QtGui>
#include <QCopChannel>

#define C_FOOTER "runcible/footer"

RWindow::RWindow(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f | Qt::FramelessWindowHint) { }

RWindow::~RWindow() { }

void RWindow::showMessage(const QString &message) {
  QByteArray data;
  QDataStream out(&data, QIODevice::WriteOnly);
  out << message;
  QCopChannel::send(C_FOOTER, "showMessage(QString)", data);
}

void RWindow::clearMessage() {
  showMessage(QString());
}

void RWindow::showTimeline(int max) {
  QByteArray data;
  QDataStream out(&data, QIODevice::WriteOnly);
  out << max;
  QCopChannel::send(C_FOOTER, "showTimeline(int)", data);
}

void RWindow::updateTimeline(int pos) {
  QByteArray data;
  QDataStream out(&data, QIODevice::WriteOnly);
  out << pos;
  QCopChannel::send(C_FOOTER, "updateTimeline(int)", data);
}

void RWindow::hideTimeline() {
  QCopChannel::send(C_FOOTER, "hideTimeline()");
}
