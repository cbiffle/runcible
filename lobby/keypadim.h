#ifndef KEYPADIM_H
#define KEYPADIM_H

#include <QWSInputMethod>

class KeypadInputMethod : public QWSInputMethod {
  Q_OBJECT

public:
  KeypadInputMethod();
  virtual ~KeypadInputMethod();

  virtual bool filter(int unicode, int keycode, int modifiers, bool isPress, bool autoPress);
  virtual void queryResponse(int property, const QVariant &result);
  virtual void reset();
  virtual void updateHandler(int type);

protected:
  char currentChar();
  void commit(const QString & = QString());
  void preedit(const QString & = QString());

private:
  enum {
    UNFOCUSED,
    COMPOSING,
    COMMITTED
  } _state;

  int _lastNumber;
  int _presses;
};
#endif
