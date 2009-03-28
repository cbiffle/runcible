#include "spawner.h"
#include <QProcess>

Spawner::Spawner(QObject *parent)
    : QObject(parent),
      _coordinator("spawner", this) { }

void Spawner::runChoice(const Choice choice) {
  QProcess::startDetached(choice.id(), choice.params());
}

void Spawner::openId(Choice choice) {
  _coordinator.openItem(QUrl(choice.id()));
}
