#ifndef BATMON_H
#define BATMON_H

#include <QObject>

class BatteryMonitor : public QObject {
  Q_OBJECT

public:
  BatteryMonitor(QObject *parent = 0);
  virtual ~BatteryMonitor();

signals:
  void chargeChanged(int pct);

public slots:
  void update();
};

#endif // BATMON_H
