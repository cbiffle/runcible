#include <QApplication>
#include <QSettings>
#include "choiceview.h"
#include "spawner.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

//  app.setStyleSheet("ChoiceView { background: white }");

  QSettings settings("runcible", "lobby");
  QString browsePath = settings.value("browse-path", "/home").toString();

  ChoiceView view;
  view.setWindowTitle("Lobby");
  view.setChoices(QList<Choice>()
      << Choice(QObject::tr("Browse"), "runcible-dir-list", QStringList() << browsePath)
      );

  Spawner spawner;

  QObject::connect(&view, SIGNAL(back()), &app, SLOT(quit()));
  QObject::connect(&view, SIGNAL(choiceMade(Choice)), &spawner, SLOT(runChoice(Choice)));

  view.showMaximized();

  return app.exec();
}
