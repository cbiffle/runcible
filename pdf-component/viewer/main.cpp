#include <QApplication>
#include <QUrl>
#include <QLayout>
#include <stdio.h>
#include <rwindow.h>
#include "pdfview.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  if (argc != 2) {
    fprintf(stderr, "Usage: runcible-view-pdf <file:url>\n");
    return 1;
  }

  QUrl path(argv[1]);

  RWindow window;

  PdfView widget;
  QObject::connect(&widget, SIGNAL(morePages(int)), &window, SLOT(showTimeline(int)));
  QObject::connect(&widget, SIGNAL(pageChanged(int)), &window, SLOT(updateTimeline(int)));
  window.layout()->addWidget(&widget);

  widget.setDocument(path.path());
  if (path.hasFragment()) {
    QString fragment = path.fragment();
    if (fragment.startsWith("page:")) {
      widget.goToPage(fragment.right(fragment.size() - 5).toInt());
    }
  }

  QObject::connect(&window, SIGNAL(back()), &app, SLOT(quit()));

  window.showMaximized();
  return app.exec();
}
