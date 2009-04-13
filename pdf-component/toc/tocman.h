#ifndef TOCMAN_H
#define TOCMAN_H

#include <QObject>
#include <QString>
#include <QList>
#include <choiceview.h>
#include <coord_client.h>

class TocManager : public QObject {
  Q_OBJECT

public:
  TocManager(const QString &path, QObject *parent = 0);
  virtual ~TocManager();

public slots:
  void refresh();
  void activate(Choice);

signals:
  void contentsChanged(const QList<Choice> &);

private:
  QString _path;
  CoordinatorClient _coordinator;
};

#endif // TOCMAN_H
