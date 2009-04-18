#include "pdfview.h"

#include <QKeyEvent>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QUrl>
#include <stdio.h>

#include <iostream>


class FitPageScaler : public ScalingStrategy {
public:
  virtual QImage scaleAndRender(const Poppler::Page *page, const QWidget *widget) {
    QSizeF pageSize = page->pageSizeF();
    qreal win = pageSize.width() / 72., hin = pageSize.height() / 72.;
    qreal xsc = widget->width() / win, ysc = widget->height() / hin;

    qreal scale = qMin(xsc, ysc);

    return page->renderToImage(scale, scale);
  }
};

class TextScaler : public ScalingStrategy {
public:
  virtual QImage scaleAndRender(const Poppler::Page *page, const QWidget *widget) {
    QList<Poppler::TextBox *> textBoxen = page->textList();
    QRectF bounds;
     while (!textBoxen.isEmpty()) {
        Poppler::TextBox *box = textBoxen.takeFirst();
        bounds = bounds.united(box->boundingBox());
        delete box;
     }

    bounds.adjust(-4, -4, 8, 8);

     qreal win = bounds.width() / 72., hin = bounds.height() / 72.;
     qreal xsc = widget->width() / win, ysc = widget->height() / hin;

    qreal scale = qMin(xsc, ysc);
    QRectF viewRect(bounds.left() / 72. * scale, bounds.top() / 72. * scale,
        bounds.width() / 72. * scale, bounds.height() / 72. * scale);

    QRect iRect = viewRect.toAlignedRect();

    return page->renderToImage(scale, scale,
         iRect.left(), iRect.top(), iRect.width(), iRect.height());
  }
};

PdfView::PdfView(QWidget *parent) 
    : RDocView(parent),
      _imagedPage(0),
      _renderScheduled(false) {
  doc = 0;
  connect(this, SIGNAL(pageChanged(int)), this, SLOT(setupPage(int)));

  scalingStrategies << new FitPageScaler() << new TextScaler();
  scalerIndex = 0;
}

PdfView::~PdfView() {
  qDeleteAll(scalingStrategies);
  delete doc;
}

Poppler::Document *PdfView::document() {
  return doc;
}

ScalingStrategy *PdfView::scaler() {
  return scalingStrategies[scalerIndex];
}

void PdfView::zoomIn() {
  if (scalerIndex < scalingStrategies.size() - 1) {
    scalerIndex++;
    _pageImage = QPixmap();
    setupPage(pageIndex());
    update();
  }
}

void PdfView::zoomOut() {
  if (scalerIndex > 0) {
    scalerIndex--;
    _pageImage = QPixmap();
    setupPage(pageIndex());
    update();
  }
}

void PdfView::resizeEvent(QResizeEvent *event) {
  RDocView::resizeEvent(event);
  _pageImage = QPixmap();
  setupPage(pageIndex());
}

void PdfView::setupPage(int newPage) {
  qDebug() << "setupPage" << newPage;
  if (_imagedPage == newPage && !_pageImage.isNull()) return;

  if (_imagedPage == (newPage - 1) && !_nextImage.isNull()) {
    qDebug() << "Flipping to _nextImage.";
    _pageImage = _nextImage;
    _nextImage = QPixmap();
  } else {
    qDebug() << "Re-rendering: _imagedPage =" << _imagedPage;
    _pageImage = QPixmap();
    _nextImage = QPixmap();
  }

  if (_pageImage.isNull()) {
    qDebug() << "Rendering current page image.";
    Poppler::Page *page = doc->page(newPage);
    QImage image = scaler()->scaleAndRender(page, this);

    _pageImage = QPixmap::fromImage(image);
    _imagedPage = newPage;
  }

  if (_nextImage.isNull() && (newPage + 1) < pageCount()) {
    if (!_renderScheduled) {
      _renderScheduled = true;
      QTimer::singleShot(0, this, SLOT(scheduleRender()));
    }
  }
}

void PdfView::scheduleRender() {
  _renderScheduled = false;
  qDebug() << "Rendering next page image.";
  Poppler::Page *page = doc->page(_imagedPage + 1);
  QImage image = scaler()->scaleAndRender(page, this);

  _nextImage = QPixmap::fromImage(image);
}

void PdfView::renderPage(int page) {
  QPainter p(this);
  p.drawPixmap(_pageImage.rect(), _pageImage, _pageImage.rect());
}

bool PdfView::setDocument(const QString &path) {
  Poppler::Document *oldDoc = doc;

  doc = Poppler::Document::load(path);
  delete oldDoc;

  if (doc) {
    doc->setRenderHint(Poppler::Document::Antialiasing);
    doc->setRenderHint(Poppler::Document::TextAntialiasing);
    contentsChanged(QUrl::fromLocalFile(path));
    _pageImage = QPixmap();
    setupPage(pageIndex());
  }

  return doc;
}

int PdfView::pageCount() {
  return doc->numPages();
}
