#ifndef CHOICEVIEW_H
#define CHOICEVIEW_H

#include <QtCore>
#include <QWidget>

class QKeyEvent;
class QLabel;
class QGridLayout;

#define NUM_CHOICES 10
/*
 * A simple value class that represents a choice in the ChoiceView.
 */
class Choice {
  public:
    Choice(const QString &id, const QString &title);

    const QString &id() const;
    const QString &title() const;

  private:
    QString _id, _title;
};

/*
 * A specialized view that presents a menu of ten Choice items
 * at a time.  Items can be selected using 0-9, or paged through
 * using Up/Down.
 */
class ChoiceView : public QWidget {
  Q_OBJECT

public:
  ChoiceView(QWidget *parent = 0);
  ~ChoiceView();

  virtual void keyPressEvent(QKeyEvent *);

public slots:
  void choose(int);
  void goBack();
  void setChoices(QList<Choice>);
  void pageUp();
  void pageDown();

signals:
  void choiceMade(const Choice);
  void back();

private:
  void refreshLabels();

  QList<Choice> _choices;
  QGridLayout *_layout;
  QList<QLabel *> _labels;
  QList<QLabel *> _hotkeys;
  int _offset;
};

#endif // CHOICEVIEW_H
