#include <QApplication>
#include <QBoxLayout>
#include <QLabel>
#include <rwindow.h>
#include <runcible-util.h>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QString filename("unknown");
  if (argc == 2) {
    filename = argv[1];
  }

  RWindow window;
  QBoxLayout *layout = qobject_cast<QBoxLayout *>(window.layout());

  QLabel heading("Could not open file.");
  heading.setFont(QFont("Liberation Serif", 30));
  layout->addWidget(&heading);

  layout->addStrut(8);

  QLabel detail(
    QString("The file %1 is of an unrecognized type and cannot be opened.")
        .arg(runcible::quote(filename)));
  detail.setFont(QFont("Liberation Serif", 12));
  detail.setWordWrap(true);
  layout->addWidget(&detail);

  layout->addStretch(1);

  QObject::connect(&window, SIGNAL(back()), &app, SLOT(quit()));

  window.showMessage("How unfortunate.");
  window.showMaximized();

  return app.exec();
}
