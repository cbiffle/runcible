#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QWidget>
class QTextDocument;
class QPaintEvent;
class QKeyEvent;
class QResizeEvent;

class PageView : public QWidget {
  Q_OBJECT

public:
  PageView(QWidget *parent = 0);
  virtual ~PageView();

public slots:
  void setDocument(QTextDocument *);
  void pageUp();
  void pageDown();
  void goToPage(int);

signals:
  void pageCountChanged(int);
  void pageChanged(int);

protected:
  virtual void paintEvent(QPaintEvent *);
  virtual void keyPressEvent(QKeyEvent *);
  virtual void resizeEvent(QResizeEvent *);

private:
  QTextDocument *_doc;
  QRect _boxRect;
  int _pageIndex;
  int _pageNumberEntry;
};

#endif // PAGEVIEW_H
