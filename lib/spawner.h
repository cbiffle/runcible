#ifndef SPAWNER_H
#define SPAWNER_H

#include <QObject>
#include "choiceview.h"

/*
 * A trivial class that runs programs described by
 * Choices.
 */
class Spawner : public QObject {
  Q_OBJECT

public slots:
  void runChoice(const Choice choice);
};

#endif
