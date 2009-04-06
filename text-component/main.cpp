#include <stdio.h>
#include <QApplication>
#include <QFile>
#include <QFileInfo>
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
  QString suffix(QFileInfo(file).suffix());

  qDebug() << "Loading" << filename;
  QString text;
  if (file.open(QIODevice::ReadOnly)) {
    qDebug() << "Opened";
    text = file.readAll();
    qDebug() << "Loaded";
  }


  RWindow window;
  PageView display;
  window.layout()->addWidget(&display);
  qDebug() << "Added display.";

  QObject::connect(&display, SIGNAL(pageCountChanged(int)), &window, SLOT(showTimeline(int)));
  QObject::connect(&display, SIGNAL(pageChanged(int)), &window, SLOT(updateTimeline(int)));

  QTextDocument doc;
  doc.setDefaultTextOption(QTextOption(Qt::AlignJustify));
  if (suffix == "html") {
    doc.setHtml(text);
  } else {
    text.replace(QRegExp("([^\\r\\n])(\\r)?\\n([^\\n\\r])"), "\\1  \\3");
    doc.setPlainText(text);
  }
  qDebug() << "Created doc.";
  display.setDocument(filename, &doc);
  qDebug() << "Set doc.";

  QObject::connect(&window, SIGNAL(back()), &app, SLOT(quit()));

  window.showMessage(doc.metaInformation(QTextDocument::DocumentTitle));
  window.showMaximized();

  return app.exec();
}
