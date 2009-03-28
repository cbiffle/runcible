#ifndef TOCMAN_H
#define TOCMAN_H

#include <QObject>
#include <QString>
#include <QList>
#include <choiceview.h>

class TocManager : public QObject {
  Q_OBJECT

public:
  TocManager(const QString &path, QObject *parent = 0);
  virtual ~TocManager();

public slots:
  void refresh();
  void activate(Choice);

signals:
  void contentsChanged(QList<Choice>);

private:
  QString _path;
};

#endif // TOCMAN_H
