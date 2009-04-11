#include "keypadim.h"

#include <QtCore>
#include <QDebug>

static struct {
  const char *chars;
  int count;
} _keyChars[] = {
  { "abc", 3 },
  { "def", 3 },
  { "ghi", 3 },
  { "jkl", 3 },
  { "mno", 3 },
  { "pqrs", 4 },
  { "tuv", 3 },
  { "wxyz", 4 },
  { " ", 1 },
};
KeypadInputMethod::KeypadInputMethod() 
    : _state(UNFOCUSED),
      _lastNumber(0),
      _presses(-1) {
}

KeypadInputMethod::~KeypadInputMethod() { }

static bool isLetterKey(int keycode) {
  return keycode >= Qt::Key_1 && keycode <= Qt::Key_8;
}
static int getLetterKey(int keycode) {
  return keycode - Qt::Key_1;
}

char KeypadInputMethod::currentChar() {
  return _keyChars[_lastNumber].chars[_presses];
}

void KeypadInputMethod::commit(const QString &str) {
  if (str.isNull()) {
    commit(QString(1, currentChar()));
  } else {
    sendCommitString(str);
    _state = COMMITTED;
  }
}

void KeypadInputMethod::preedit(const QString &str) {
  if (str.isNull()) {
    preedit(QString(1, currentChar()));
  } else {
    sendPreeditString(str, str.size());
    _state = COMPOSING;
  }
}

bool KeypadInputMethod::filter(int unicode, int keycode, int modifiers, bool isPress, bool autoRepeat) {
  if (!isPress) return QWSInputMethod::filter(unicode, keycode, modifiers, isPress, autoRepeat);

  switch (_state) {
    case UNFOCUSED:
      return QWSInputMethod::filter(unicode, keycode, modifiers, isPress, autoRepeat);

    case COMPOSING:
      if (isLetterKey(keycode)) {
        int n = getLetterKey(keycode);
        if (n != _lastNumber) {
          commit();
          _lastNumber = n;
          _presses = -1;
        }
        _presses = (_presses + 1) % _keyChars[_lastNumber].count;
        preedit();
        return true;
      } else if (keycode == Qt::Key_9) {
        commit();
        return true;
      } else if (keycode == Qt::Key_0) {
        commit();
        commit(" ");
        return true;
      } else if (keycode == Qt::Key_Escape) {
        commit("");
        return true;
      } else if (keycode == Qt::Key_Return) {
        commit();
        return false; // handle event elsewhere.
      }
      break;

    case COMMITTED:
      if (isLetterKey(keycode)) {
        int n = getLetterKey(keycode);
        _lastNumber = n;
        _presses = 0;
        preedit();
        return true;
      } else if (keycode == Qt::Key_0) {
        commit(" ");
        return true;
      } else if (keycode == Qt::Key_9) {
        return true;
      } else if (keycode == Qt::Key_Escape) {
        if (!(modifiers && Qt::AltModifier)) {
          QWSServer::sendKeyEvent(8, Qt::Key_Backspace, 0, true, false);
          return true;
        }
      } else if (keycode == Qt::Key_Return) {
        return false; // handle event elsewhere.
      }
      break;
  }
  return QWSInputMethod::filter(unicode, keycode, modifiers, isPress, autoRepeat);
}

void KeypadInputMethod::queryResponse(int property, const QVariant &result) {
  qDebug() << "queryResponse" << property << result;
  QWSInputMethod::queryResponse(property, result);
}
void KeypadInputMethod::reset() {
  qDebug() << "reset";
  QWSInputMethod::reset();
}

void KeypadInputMethod::updateHandler(int type) {
  switch (type) {
    case QWSInputMethod::FocusIn:
      _state = COMMITTED;
      break;
    case QWSInputMethod::FocusOut:
      _state = UNFOCUSED;
      break;
      
    case QWSInputMethod::Update:
    case QWSInputMethod::Reset:
    case QWSInputMethod::Destroyed:
      break;
  }
}
