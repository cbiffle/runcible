#include <QApplication>
#include <QSettings>
#include <QWSServer>
#include <QFont>
#include <rwindow.h>
#include "choiceview.h"
#include "spawner.h"
#include "footer.h"
#include "keyguard.h"
#include "coord-server.h"
#include "batmon.h"
#include "keypadim.h"

class GuardHandler : public QWSServer::KeyboardFilter {
public:
  KeyGuard *guard;
  GuardHandler() {
    guard = new KeyGuard();
  }
  virtual ~GuardHandler() {
    delete guard;
  }
  virtual bool filter(int, int keycode, int, bool isPress, bool autoRepeat) {
    if (keycode == Qt::Key_Standby && isPress && !autoRepeat) {
      if (guard->isVisible()) {
        guard->hide();
      } else {
        guard->showMaximized();
      }
      return true;
    }
    return false;
  };

  
};
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  CoordinatorServer server;

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

  GuardHandler filter;
  QWSServer::addKeyboardFilter(&filter);
  QWSServer::setCurrentInputMethod(new KeypadInputMethod);

  RWindow window;
  window.showMessage("Lobby");

  ChoiceView view(&window);
  window.layout()->addWidget(&view);
  view.setChoices(QList<Choice>()
      << Choice(QObject::tr("Browse"), browseUrl.toString())
      );

  Spawner spawner;
  BatteryMonitor batmon;

  QObject::connect(&view, SIGNAL(back()), &app, SLOT(quit()));
  QObject::connect(&view, SIGNAL(choiceMade(Choice)), &spawner, SLOT(openId(Choice)));

  QObject::connect(QWSServer::instance(), SIGNAL(windowEvent(QWSWindow *, QWSServer::WindowEvent)),
      &statusBar, SLOT(windowEvent(QWSWindow *, QWSServer::WindowEvent)));

  QObject::connect(&batmon, SIGNAL(chargeChanged(int)), &statusBar, SLOT(updateCharge(int)));

  window.setWindowTitle("Lobby");
  window.showMaximized();

  return app.exec();
}
