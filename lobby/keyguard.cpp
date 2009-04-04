#include "keyguard.h"
#include <QKeyEvent>
#include <QLabel>
#include <QPixmap>

KeyGuard::KeyGuard(QWidget *parent) : QLabel(parent) {
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  setWindowTitle("Keyboard Locked - Press Power");
  setPixmap(QPixmap("/usr/local/runcible-startup-4.png"));
}
KeyGuard::~KeyGuard() { }

void KeyGuard::keyPressEvent(QKeyEvent *event) {
}
