#ifndef COORD_CLIENT_H
#define COORD_CLIENT_H

#include <QtCore>

class CoordinatorClient : public QObject {
  Q_OBJECT

public:
  CoordinatorClient(const QString &clientName, QObject *parent = 0);
  virtual ~CoordinatorClient();

  void openItem(QUrl);
};

#endif // COORD_CLIENT_H
