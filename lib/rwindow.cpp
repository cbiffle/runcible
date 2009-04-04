#include "rwindow.h"

#include <QtGui>
#include <QCopChannel>
#include <QKeyEvent>

#define C_FOOTER "runcible/footer"

RWindow::RWindow(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f | Qt::FramelessWindowHint) {
  setLayout(new QVBoxLayout(this));
}

RWindow::~RWindow() { }

void RWindow::showMessage(const QString &message) {
  QByteArray data;
  QDataStream out(&data, QIODevice::WriteOnly);
  out << (int) winId() << message;
  QCopChannel::send(C_FOOTER, "showMessage(int,QString)", data);
}

void RWindow::clearMessage() {
  showMessage(QString());
}

void RWindow::showTimeline(int max) {
  QByteArray data;
  QDataStream out(&data, QIODevice::WriteOnly);
  out << (int) winId() << max;
  QCopChannel::send(C_FOOTER, "showTimeline(int,int)", data);
}

void RWindow::updateTimeline(int pos) {
  QByteArray data;
  QDataStream out(&data, QIODevice::WriteOnly);
  out << (int) winId() << pos;
  QCopChannel::send(C_FOOTER, "updateTimeline(int,int)", data);
}

void RWindow::hideTimeline() {
  QByteArray data;
  QDataStream out(&data, QIODevice::WriteOnly);
  out << (int) winId();
  QCopChannel::send(C_FOOTER, "hideTimeline(int)", data);
}

void RWindow::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Escape) {
    emit back();
  } else {
    QWidget::keyPressEvent(event);
  }
}
