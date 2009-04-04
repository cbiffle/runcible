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

Footer::Footer(QWidget *parent)
    : QWidget(parent),
      _currentWinId(0),
      _qcop("runcible/footer", this) {

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
  _message->setFont(QFont("Liberation Serif", 13, QFont::Black));

  layout->addWidget(_message, 0, 0);
  layout->addWidget(_progBar, 0, 1);
  layout->setColumnStretch(0, 1);
  setLayout(layout);

  QPalette palette;
  palette.setColor(QPalette::Background, Qt::black);
  palette.setColor(QPalette::Foreground, Qt::white);
  palette.setColor(QPalette::Highlight, Qt::white);
  setPalette(palette);

  connect(&_qcop, SIGNAL(received(const QString &, const QByteArray &)),
      this, SLOT(received(const QString &, const QByteArray &)));
}

Footer::~Footer() {}

FooterState &Footer::state(int winId) {
  if (winId == -1) winId = _currentWinId;

  if (winId == 0) {
    return _default;
  }
  return _windowStates[winId];
}

void Footer::showTimeline(int winId, int max) {
  FooterState &s = state(winId);
  s.timelineMax = max;
  s.timelinePos = 0;
  s.timelineVisible = true;
  updateState(winId);
}

void Footer::updateTimeline(int winId, int pos) {
  state(winId).timelinePos = pos;
  updateState(winId);
}

void Footer::hideTimeline(int winId) {
  state(winId).timelineVisible = false;
  updateState(winId);
}

void Footer::showMessage(int winId, const QString &message) {
  state(winId).message = message;
  updateState(winId);
}

void Footer::clearMessage(int winId) {
  state(winId).message = QString();
  updateState(winId);
}

void Footer::windowEvent(QWSWindow *window, QWSServer::WindowEvent event) {
  switch (event) {
    case QWSServer::Create:
      _windowStates.insert(window->winId(), FooterState());
      break;

    case QWSServer::Active:
      _currentWinId = window->winId();
      QTimer::singleShot(250, this, SLOT(updateState()));
      break;

    case QWSServer::Destroy:
      _windowStates.remove(window->winId());
      break;

    default: break;
  }
}

void Footer::updateState() {
  updateState(_currentWinId);
}

void Footer::updateState(int winId) {
  if (winId != _currentWinId) return;

  const FooterState &s = state(winId);
  if (s.message != _message->text()) _message->setText(s.message);
  if (s.timelineMax != _progBar->maximum()) _progBar->setMaximum(s.timelineMax);
  if (s.timelinePos != _progBar->value()) _progBar->setValue(s.timelinePos);
  if (s.timelineVisible != _progBar->isVisible()) _progBar->setVisible(s.timelineVisible);
}

void Footer::received(const QString &message, const QByteArray &data) {
  QDataStream in(data);
  int winId;
  if (message == "showMessage(int,QString)") {
    QString msg;
    in >> winId >> msg;
    showMessage(winId, msg);
  } else if (message == "showTimeline(int,int)") {
    int max;
    in >> winId >> max;
    showTimeline(winId, max);
  } else if (message == "updateTimeline(int,int)") {
    int pos;
    in >> winId >> pos;
    updateTimeline(winId, pos);
  } else if (message == "hideTimeline(int)") {
    in >> winId;
    hideTimeline(winId);
  } else {
    qDebug() << "Message not recognized:" << message;
  }
}


