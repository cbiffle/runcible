#ifndef SPAWNER_H
#define SPAWNER_H

#include <QObject>
#include "choiceview.h"
#include "coord_client.h"

/*
 * A trivial class that runs programs described by
 * Choices.
 */
class Spawner : public QObject {
  Q_OBJECT

public:
  Spawner(QObject *parent = 0);

public slots:
  void runChoice(const Choice choice);
  void openId(Choice);

private:
  CoordinatorClient _coordinator;
};

#endif
