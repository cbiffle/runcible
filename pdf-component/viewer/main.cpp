#include <QApplication>
#include <QUrl>
#include <stdio.h>
#include "pdfview.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  if (argc != 2) {
    fprintf(stderr, "Usage: runcible-view-pdf <file:url>\n");
    return 1;
  }

  QUrl path(argv[1]);

  PdfView widget;
  widget.setDocument(path.path());
  if (path.hasFragment()) {
    QString fragment = path.fragment();
    if (fragment.startsWith("page:")) {
      widget.showPage(fragment.right(fragment.size() - 5).toInt());
    }
  }

  QObject::connect(&widget, SIGNAL(back()), &app, SLOT(quit()));

  widget.setWindowFlags(Qt::FramelessWindowHint);
  widget.showMaximized();
  return app.exec();
}
