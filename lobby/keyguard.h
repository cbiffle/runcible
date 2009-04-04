#ifndef KEYGUARD_H
#define KEYGUARD_H

#include <QLabel>
class QKeyEvent;

class KeyGuard : public QLabel {
  Q_OBJECT

public:
  KeyGuard(QWidget *parent = 0);
  virtual ~KeyGuard();

protected:
  virtual void keyPressEvent(QKeyEvent *);
};

#endif // KEYGUARD_H
