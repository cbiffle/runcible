#include <QApplication>
#include <QSettings>
#include "choiceview.h"
#include "spawner.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

//  app.setStyleSheet("ChoiceView { background: white }");

  QSettings settings("runcible", "lobby");
  QString browsePath = settings.value("browse-path", "/home").toString();
  QUrl browseUrl("file:" + browsePath);

  ChoiceView view;
  view.setWindowTitle("Lobby");
  view.setChoices(QList<Choice>()
      << Choice(QObject::tr("Browse"), browseUrl.toString())
      );

  Spawner spawner;

  QObject::connect(&view, SIGNAL(back()), &app, SLOT(quit()));
  QObject::connect(&view, SIGNAL(choiceMade(Choice)), &spawner, SLOT(openId(Choice)));

  view.showMaximized();

  return app.exec();
}
