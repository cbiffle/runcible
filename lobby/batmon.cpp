#include "batmon.h"
#include <QTimer>
#include <QFile>

BatteryMonitor::BatteryMonitor(QObject *parent) : QObject(parent) {
  QTimer *timer = new QTimer(this);

  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(30000);
}

BatteryMonitor::~BatteryMonitor() {}

void BatteryMonitor::update() {
  QFile charge("/sys/class/power_supply/lbookv3_battery/charge_now");
  if (charge.open(QIODevice::ReadOnly)) {
    QString contents = charge.readAll();
    int pct = contents.toInt();
    emit chargeChanged(pct);
  }
}
