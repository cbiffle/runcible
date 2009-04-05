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

protected:
  virtual void paintEvent(QPaintEvent *);
  virtual void keyPressEvent(QKeyEvent *);
  virtual void resizeEvent(QResizeEvent *);

private:
  QTextDocument *_doc;
  int _pageIndex;
};

#endif // PAGEVIEW_H
