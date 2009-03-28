#ifndef PDFVIEW_H
#define PDFVIEW_H

#include <QLabel>
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
class PdfView : public QLabel {
  Q_OBJECT

public:
  PdfView(QWidget *parent = 0);
  ~PdfView();

  Poppler::Document *document();

  void keyPressEvent(QKeyEvent *event);
  void resizeEvent(QResizeEvent *event);

public slots:
  bool setDocument(const QString &path);
  void zoomIn();
  void zoomOut();
  
signals:
  void pageChanged(int);
  void back();

private:
  void showPage(int);
  ScalingStrategy *scaler();

  Poppler::Document *doc;

  int pageIndex;
  int pageIndexEntry;

  int scalerIndex;
  QList<ScalingStrategy *> scalingStrategies;
};

#endif // PDFVIEW_H