#ifndef PDFVIEW_H
#define PDFVIEW_H

#include <rdocview.h>
#include <QList>
#include <poppler-qt4.h>

class QWidget;

/*
 * Knows how to scale a page to fit in a widget.  Currently, all
 * implementations are hardcoded in pdfview.cpp.
 */
class ScalingStrategy {
public:
  virtual QImage scaleAndRender(const Poppler::Page *page, const QWidget *widget) = 0;
};

/*
 * A widget that can display pages of a PDF sequentially.  Derived
 * from Trolltech's Poppler PDF example (Qt Quarterly 27).
 */
class PdfView : public RDocView {
  Q_OBJECT

public:
  PdfView(QWidget *parent = 0);
  ~PdfView();

  Poppler::Document *document();

  virtual void resizeEvent(QResizeEvent *event);
  virtual int pageCount();

public slots:
  bool setDocument(const QString &path);
  virtual void zoomIn();
  virtual void zoomOut();

  void setupPage(int);
  
protected:
  virtual void renderPage(int page);

private:
  ScalingStrategy *scaler();

  Poppler::Document *doc;
  QPixmap _pageImage;

  int scalerIndex;
  QList<ScalingStrategy *> scalingStrategies;
};

#endif // PDFVIEW_H
