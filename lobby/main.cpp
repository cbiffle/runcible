#include <QApplication>
#include <QSettings>
#include <QLabel>
#include <QWSServer>
#include <QProgressBar>
#include "choiceview.h"
#include "spawner.h"
#include "footer.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QRect max(0, 0, 600, 800 - 26);
  QWSServer::setMaxWindowRect(max);

//  app.setStyleSheet("ChoiceView { background: white }");

  QSettings settings("runcible", "lobby");
  QString browsePath = settings.value("browse-path", "/home").toString();
  QUrl browseUrl("file:" + browsePath);

  Footer statusBar;
  statusBar.setWindowFlags(Qt::FramelessWindowHint);
  statusBar.move(0, 800-26);
  statusBar.resize(600, 26);

  statusBar.show();

  ChoiceView view;
  view.setWindowFlags(Qt::FramelessWindowHint);
  view.setChoices(QList<Choice>()
      << Choice(QObject::tr("Browse"), browseUrl.toString())
      );

  Spawner spawner;

  QObject::connect(&view, SIGNAL(back()), &app, SLOT(quit()));
  QObject::connect(&view, SIGNAL(choiceMade(Choice)), &spawner, SLOT(openId(Choice)));
  QObject::connect(QWSServer::instance(), SIGNAL(windowEvent(QWSWindow *, QWSServer::WindowEvent)),
      &statusBar, SLOT(windowEvent(QWSWindow *, QWSServer::WindowEvent)));

  view.setWindowTitle("Lobby");
  view.setWindowFlags(Qt::FramelessWindowHint);
  view.showMaximized();

  return app.exec();
}
