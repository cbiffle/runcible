#ifndef COORD_SERVER_H
#define COORD_SERVER_H

#include <QtCore>
#include <QtGui>
#include <QCopChannel>

class CoordinatorServer : public QObject {
  Q_OBJECT

public:
  CoordinatorServer(QObject *parent = 0);
  virtual ~CoordinatorServer();

public slots:
  void received(const QString &message, const QByteArray &data);

private:
  void openItem(QUrl url);
  void openItem(QString program, QUrl url);

  QCopChannel _c;
};

#endif // COORD_SERVER_H
