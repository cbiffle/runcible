#ifndef RDOCVIEW_H
#define RDOCVIEW_H

#include <QWidget>
class QPaintEvent;
class QKeyEvent;
class QResizeEvent;

class RDocView : public QWidget {
  Q_OBJECT

public:
  RDocView(QWidget *parent = 0);
  virtual ~RDocView();

  int pageIndex();

public slots:
  void pageUp();
  void pageDown();
  void goToPage(int);
  virtual void zoomIn();
  virtual void zoomOut();

signals:
  void pageCountChanged(int);
  void pageChanged(int);

protected:
  virtual void paintEvent(QPaintEvent *);
  virtual void keyPressEvent(QKeyEvent *);
  virtual void resizeEvent(QResizeEvent *);

  virtual void renderPage(int) = 0;
  virtual int pageCount() = 0;

  void reset();
  
private:
  QRect _boxRect;
  int _pageIndex;
  int _pageNumberEntry;
};

#endif // RDOCVIEW_H
