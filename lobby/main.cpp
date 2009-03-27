#include <QApplication>
#include "choiceview.h"
#include "spawner.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

//  app.setStyleSheet("ChoiceView { background: white }");

  ChoiceView view;
  view.setWindowTitle("Lobby");
  view.setChoices(QList<Choice>()
      << Choice(QObject::tr("Browse"), "runcible-dir-list")
      );

  Spawner spawner;

  QObject::connect(&view, SIGNAL(back()), &app, SLOT(quit()));
  QObject::connect(&view, SIGNAL(choiceMade(Choice)), &spawner, SLOT(runChoice(Choice)));

  view.showMaximized();

  return app.exec();
}
