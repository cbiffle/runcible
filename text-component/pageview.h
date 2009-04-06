#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <rdocview.h>

class QTextDocument;
class QPaintEvent;
class QKeyEvent;
class QResizeEvent;

class PageView : public RDocView {
  Q_OBJECT

public:
  PageView(QWidget *parent = 0);
  virtual ~PageView();

public slots:
  void setDocument(const QString &path, QTextDocument *);
  virtual void zoomIn();
  virtual void zoomOut();

protected:
  virtual void renderPage(int);
  virtual int pageCount();
  virtual void resizeEvent(QResizeEvent *);

private:
  QTextDocument *_doc;
};

#endif // PAGEVIEW_H
