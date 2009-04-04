#include <QApplication>
#include <QLayout>
#include <choiceview.h>
#include <rwindow.h>
#include "dirman.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QString path;
  if (argc > 1) {
    path = argv[1];
  } else {
    path = "/";
  }

  DirManager dirman(path);
  RWindow window;
  ChoiceView view;
  window.layout()->addWidget(&view);

  QObject::connect(&view, SIGNAL(back()), &app, SLOT(quit()));
  QObject::connect(&view, SIGNAL(choiceMade(Choice)), &dirman, SLOT(activate(Choice)));
  QObject::connect(&dirman, SIGNAL(contentsChanged(QList<Choice>)), &view, SLOT(setChoices(QList<Choice>)));

  dirman.refresh();
  window.showMaximized();
  window.showMessage(path);

  return app.exec();
}
