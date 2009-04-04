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

FooterState &Footer::state() {
  if (_currentWindow == 0) {
    return _default;
  }
  return _windowStates[_currentWindow];
}

void Footer::showTimeline(int max) {
  qDebug() << "showTimeline" << max << _currentWindow;
  state().timelineMax = max;
  state().timelinePos = 0;
  state().timelineVisible = true;
  updateState();
}

void Footer::updateTimeline(int pos) {
  qDebug() << "updateTimeline" << pos << _currentWindow;
  state().timelinePos = pos;
  updateState();
}

void Footer::hideTimeline() {
  qDebug() << "hideTimeline" << _currentWindow;
  state().timelineVisible = false;
  updateState();
}

void Footer::showMessage(const QString &message) {
  state().message = message;
  updateState();
}

void Footer::clearMessage() {
  state().message = QString();
  updateState();
}

void Footer::windowEvent(QWSWindow *window, QWSServer::WindowEvent event) {
  switch (event) {
    case QWSServer::Create:
      qDebug() << "initial footer for" << window;
      _windowStates.insert(window, FooterState());
      break;

    case QWSServer::Active:
      qDebug() << "showing footer for" << window;
      _currentWindow = window;
      QTimer::singleShot(250, this, SLOT(updateState()));
      break;

    case QWSServer::Destroy:
      qDebug() << "nuking footer for" << window;
      _windowStates.remove(window);
      break;

    default: break;
  }
  qDebug() << "winid =" << window->winId();
}

void Footer::updateState() {
  const FooterState &s = state();
  if (s.message != _message->text()) _message->setText(s.message);
  if (s.timelineMax != _progBar->maximum()) _progBar->setMaximum(s.timelineMax);
  if (s.timelinePos != _progBar->value()) _progBar->setValue(s.timelinePos);
  if (s.timelineVisible != _progBar->isVisible()) _progBar->setVisible(s.timelineVisible);
}

void Footer::received(const QString &message, const QByteArray &data) {
  qDebug() << "Footer received" << message;
  QDataStream in(data);
  if (message == "showMessage(QString)") {
    QString msg;
    in >> msg;
    showMessage(msg);
  } else if (message == "showTimeline(int)") {
    int max;
    in >> max;
    showTimeline(max);
  } else if (message == "updateTimeline(int)") {
    int pos;
    in >> pos;
    updateTimeline(pos);
  } else if (message == "hideTimeline()") {
    hideTimeline();
  } else {
    qDebug() << "Message not recognized:" << message;
  }
}


