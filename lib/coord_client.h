#ifndef COORD_CLIENT_H
#define COORD_CLIENT_H

#include <QtCore>

/*
 * A client for the Runcible Coordinator, which dispatches requests
 * across components.
 */
class CoordinatorClient : public QObject {
  Q_OBJECT

public:
  CoordinatorClient(const QString &clientName, QObject *parent = 0);
  virtual ~CoordinatorClient();

public slots:
  void openItem(QUrl);
};

#endif // COORD_CLIENT_H
