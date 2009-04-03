#include "footer.h"

#include <QGridLayout>
#include <QTimer>

FooterState::FooterState()
    : timelineVisible(false),
      timelineMax(0),
      timelinePos(0),
      message()
{
}

Footer::Footer(QWidget *parent) : QWidget(parent) {
  QGridLayout *layout = new QGridLayout(this);
  layout->setSpacing(2);
  layout->setContentsMargins(1,1,1,1);

  _progBar = new QProgressBar;
  _progBar->setMaximumHeight(20);
  _progBar->setMaximum(1);
  _progBar->setTextVisible(false);
  _progBar->hide();

  _message = new QLabel();
  _message->setMaximumHeight(20);

  layout->addWidget(_message, 0, 0);
  layout->addWidget(_progBar, 0, 1);
  layout->setColumnStretch(0, 1);
  setLayout(layout);

  QPalette palette;
  palette.setColor(QPalette::Background, Qt::black);
  palette.setColor(QPalette::Foreground, Qt::white);
  palette.setColor(QPalette::Highlight, Qt::white);
  setPalette(palette);
}

Footer::~Footer() {}

FooterState &Footer::state() {
  return _default;
}

void Footer::showTimeline(int max) {
  _progBar->setMaximum(max == 0? 1 : max);
  _progBar->setValue(0);
  _progBar->show();
}

void Footer::updateTimeline(int pos) {
  _progBar->setValue(pos);
}

void Footer::hideTimeline() {
  _progBar->hide();
}

void Footer::showMessage(const QString &message) {
  _message->setText(message);
}
void Footer::clearMessage() {
  _message->setText(QString());
}

void Footer::windowEvent(QWSWindow *window, QWSServer::WindowEvent event) {
  if (event == QWSServer::Active) {
    state().message = window->caption();
    QTimer::singleShot(250, this, SLOT(updateState()));
  }
}

void Footer::updateState() {
  showMessage(state().message);
}
