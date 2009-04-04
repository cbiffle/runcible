#ifndef FOOTER_H
#define FOOTER_H

#include <QtCore>
#include <QWidget>
#include <QWSWindow>
#include <QProgressBar>
#include <QLabel>
#include <QCopChannel>

class FooterState {
public:
  FooterState();

  bool timelineVisible;
  int timelineMax;
  int timelinePos;

  QString message;
};

class Footer : public QWidget {
  Q_OBJECT

public:
  Footer(QWidget *parent = 0);
  virtual ~Footer();

public slots:
  void showTimeline(int max);
  void hideTimeline();

  void updateTimeline(int pos);

  void showMessage(const QString &message);
  void clearMessage();

  void windowEvent(QWSWindow *window, QWSServer::WindowEvent eventType);

  void updateState();

  void received(const QString &message, const QByteArray &data);

private:
  FooterState &state();

  QProgressBar *_progBar;
  QLabel *_message;
  QWSWindow *_currentWindow;

  FooterState _default;

  QHash<QWSWindow *, FooterState> _windowStates;

  QCopChannel _qcop;

};

#endif // FOOTER_H
