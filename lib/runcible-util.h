#ifndef RUNCIBLE_UTIL_H
#define RUNCIBLE_UTIL_H

class QString;

namespace runcible {

  /*
   * Returns the argument wrapped in typographical ("smart") English
   * double quotation marks.
   */
  QString doubleQuote(const QString &);
  /*
   * Returns the argument wrapped in typographical ("smart") English
   * single quotation marks.
   */
  QString quote(const QString &);

};
#endif // RUNCIBLE_UTIL_H
