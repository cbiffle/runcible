#include <QApplication>
#include <QLayout>
#include <rwindow.h>
#include <choiceview.h>
#include "tocman.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  if (argc != 2) {
    fprintf(stderr, "Usage: runcible-contents-pdf <file:url>");
    return -1;
  }

  QUrl url(argv[1]);

  TocManager tocman(url.path());
  RWindow window;
  ChoiceView view;

  window.layout()->addWidget(&view);

  window.showMessage("Contents of " + QFileInfo(url.path()).fileName());

  QObject::connect(&window, SIGNAL(back()), &app, SLOT(quit()));
  QObject::connect(&view, SIGNAL(choiceMade(Choice)), &tocman, SLOT(activate(Choice)));
  QObject::connect(&tocman, SIGNAL(contentsChanged(const QList<Choice> &)), &view, SLOT(setChoices(const QList<Choice> &)));

  tocman.refresh();
  window.showMaximized();

  return app.exec();
}
