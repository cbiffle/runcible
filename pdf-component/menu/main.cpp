#include <QApplication>
#include <QSettings>
#include <choiceview.h>
#include <spawner.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  if (argc != 2) {
    fprintf(stderr, "Usage: runcible-open-ext-pdf <file>");
  }

  QString file(argv[1]);

  ChoiceView view;
  view.setChoices(QList<Choice>()
      << Choice(QObject::tr("Read"), "runcible-view-pdf", QStringList() << ("file:" + file))
      << Choice(QObject::tr("Table of Contents"), "runcible-contents-pdf", QStringList() << file)
      );

  Spawner spawner;

  QObject::connect(&view, SIGNAL(back()), &app, SLOT(quit()));
  QObject::connect(&view, SIGNAL(choiceMade(Choice)), &spawner, SLOT(runChoice(Choice)));

  view.showMaximized();

  return app.exec();
}
