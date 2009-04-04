#ifndef RWINDOW_H
#define RWINDOW_H

#include <QWidget>

class QKeyEvent;

class RWindow : public QWidget {
  Q_OBJECT

public:
  RWindow(QWidget *parent = 0, Qt::WindowFlags f = 0);
  virtual ~RWindow();

public slots:
  void showMessage(const QString &);
  void clearMessage();

  void showTimeline(int max);
  void updateTimeline(int pos);
  void hideTimeline();

signals:
  void back();

protected:
  virtual void keyPressEvent(QKeyEvent *);
};

#endif // RWINDOW_H
