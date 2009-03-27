#include "spawner.h"
#include <QProcess>

void Spawner::runChoice(const Choice choice) {
  QProcess::startDetached(choice.id(), QStringList());
}
