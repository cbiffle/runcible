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
  void updateState();
  void showTimeline(int winId, int max);
  void hideTimeline(int winId);

  void updateTimeline(int winId, int pos);

  void showMessage(int winId, const QString &message);
  void clearMessage(int winId);

  void windowEvent(QWSWindow *window, QWSServer::WindowEvent eventType);


  void received(const QString &message, const QByteArray &data);

private:
  FooterState &state(int winId = -1);
  void deferredUpdate();
  bool _updateScheduled;

  QLabel *_timeline;
  QLabel *_message;
  int _currentWinId;

  FooterState _default;

  QHash<int, FooterState> _windowStates;

  QCopChannel _qcop;

};

#endif // FOOTER_H
