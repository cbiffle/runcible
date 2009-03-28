#include <QApplication>
#include <stdio.h>
#include "pdfview.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  if (argc != 2) {
    fprintf(stderr, "Usage: runcible-view-pdf <file>\n");
    return 1;
  }

  PdfView widget;
  widget.setDocument(argv[1]);
  widget.resize(600, 800);

  QObject::connect(&widget, SIGNAL(back()), &app, SLOT(quit()));

  widget.show();
  return app.exec();
}
