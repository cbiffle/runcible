#include "pdfview.h"

#include <QKeyEvent>
#include <QPainter>
#include <QDebug>
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

/*    viewRect.adjust(
        widget->width() - viewRect.width() / 2,
        widget->height() - viewRect.height() / 2,
        widget->width() - viewRect.width(),
        widget->height() - viewRect.height());*/

    return page->renderToImage(scale, scale,
         viewRect.left(), viewRect.top(), viewRect.width(), viewRect.height());
  }
};

PdfView::PdfView(QWidget *parent) 
    : QLabel(parent) {
  pageIndex = 0;
  pageIndexEntry = 0;
  doc = 0;

  scalingStrategies << new FitPageScaler() << new TextScaler();
  scalerIndex = 0;
}

PdfView::~PdfView() {
  while (!scalingStrategies.isEmpty()) {
    delete scalingStrategies.takeFirst();
  }

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
    showPage(pageIndex);
  }
}

void PdfView::zoomOut() {
  if (scalerIndex > 0) {
    scalerIndex--;
    showPage(pageIndex);
  }
}

void PdfView::resizeEvent(QResizeEvent *event) {
  QLabel::resizeEvent(event);
  showPage(pageIndex);
}

void PdfView::showPage(int newPage) {
  if (newPage != pageIndex) {
    pageIndex = newPage;
    emit pageChanged(newPage);
  }

  Poppler::Page *page = doc->page(newPage);
  QImage image = scaler()->scaleAndRender(page, this);

  setPixmap(QPixmap::fromImage(image));
}

void PdfView::paintEvent(QPaintEvent *event) {
  QLabel::paintEvent(event);

  if (pageIndexEntry > 0) {
    QPainter painter;
    painter.begin(this);
    QFont font("Arial", 72);
    painter.setFont(font);
    painter.drawText(0, 200, "Goto page:");
    painter.drawText(0, 400, QString::number(pageIndexEntry));

    painter.end();
  }
}

bool PdfView::setDocument(const QString &path) {
  Poppler::Document *oldDoc = doc;

  doc = Poppler::Document::load(path);
  delete oldDoc;

  if (doc) {
    doc->setRenderHint(Poppler::Document::Antialiasing);
    doc->setRenderHint(Poppler::Document::TextAntialiasing);
    showPage(0);
  }

  return doc;
}

void PdfView::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Down && pageIndex < doc->numPages()) {
    showPage(pageIndex + 1);
  } else if (event->key() == Qt::Key_Up && pageIndex > 0) {
    showPage(pageIndex - 1);
  } else if (event->key() == Qt::Key_Plus) {
    zoomIn();
  } else if (event->key() == Qt::Key_Minus) {
    zoomOut();
  } else if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
    pageIndexEntry = (pageIndexEntry * 10) + (event->key() - Qt::Key_0);
    update();
  } else if (event->key() == Qt::Key_Escape) {
    emit back();
  } else if (event->key() == Qt::Key_Return && pageIndexEntry > 0) {
    showPage(pageIndexEntry - 1);
    pageIndexEntry = 0;
  } else {
    QLabel::keyPressEvent(event);
  }
}
