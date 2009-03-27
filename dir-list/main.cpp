#include <QApplication>
#include <choiceview.h>
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
  ChoiceView view;

  view.setWindowTitle("Contents of " + QFileInfo(path).fileName());

  QObject::connect(&view, SIGNAL(back()), &app, SLOT(quit()));
  QObject::connect(&view, SIGNAL(choiceMade(Choice)), &dirman, SLOT(activate(Choice)));
  QObject::connect(&dirman, SIGNAL(contentsChanged(QList<Choice>)), &view, SLOT(setChoices(QList<Choice>)));

  dirman.refresh();
  view.showMaximized();

  return app.exec();
}
