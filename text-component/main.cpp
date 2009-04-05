#include <stdio.h>
#include <QApplication>
#include <QFile>
#include <QLayout>
#include <QDebug>
#include <QRegExp>
#include <QTextDocument>
#include <QTextOption>
#include <rwindow.h>
#include "pageview.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setFont(QFont("Liberation Serif", 12));

  if (argc != 2) {
    fprintf(stderr, "Usage: runcible-open-ext-txt <file>");
    return 1;
  }

  QString filename(argv[1]);
  QFile file(filename);
  qDebug() << "Loading" << filename;
  QString text;
  if (file.open(QIODevice::ReadOnly)) {
    qDebug() << "Opened";
    text = file.readAll();
    qDebug() << "Loaded";
  }

  text.replace(QRegExp("([^\\r\\n])(\\r)?\\n([^\\n\\r])"), "\\1  \\3");

  RWindow window;
  PageView display;
  window.layout()->addWidget(&display);
  qDebug() << "Added display.";

  QTextDocument doc;
  doc.setDefaultTextOption(QTextOption(Qt::AlignJustify));
  doc.setPlainText(text);
  qDebug() << "Created doc.";
  display.setDocument(&doc);
  qDebug() << "Set doc.";

  QObject::connect(&window, SIGNAL(back()), &app, SLOT(quit()));

  qDebug() << "Showing...";
  window.showMaximized();

  return app.exec();
}
