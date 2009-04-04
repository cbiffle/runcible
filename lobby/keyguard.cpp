#include "keyguard.h"
#include <QKeyEvent>
#include <QLabel>
#include <QPixmap>
#include <QLayout>

KeyGuard::KeyGuard(QWidget *parent) : RWindow(parent, Qt::WindowStaysOnTopHint) {
  showMessage("Keyboard Locked - Press Power");
  
  QLabel *pixmap = new QLabel;
  pixmap->setPixmap(QPixmap("/usr/local/runcible-startup-4.png"));
  layout()->addWidget(pixmap);
}
KeyGuard::~KeyGuard() { }

void KeyGuard::keyPressEvent(QKeyEvent *event) {
  // nom nom nom
}
