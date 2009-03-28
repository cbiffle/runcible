#ifndef DIRMAN_H
#define DIRMAN_H

#include <choiceview.h>
#include <coord_client.h>

/*
 * Manages the contents of a directory, cooperating with
 * a ChoiceView via signals and slots.
 */
class DirManager : public QObject {
  Q_OBJECT

public:
  DirManager(const QString &path, QObject *parent = 0);
  ~DirManager();

public slots:
  void activate(const Choice);
  void refresh();

signals:
  void contentsChanged(QList<Choice>);

private:
  QString _basePath;
  CoordinatorClient _coordinator;

};

#endif // DIRMAN_H
