#include "runcible-util.h"

#include <QtCore>

namespace runcible {

QString doubleQuote(const QString &body) {
  QString res(QChar(0x201C));
  res += body;
  res += QChar(0x201D);
  return res;
}

QString quote(const QString &body) {
  QString res(QChar(0x2018));
  res += body;
  res += QChar(0x2019);
  return res;
}

};
