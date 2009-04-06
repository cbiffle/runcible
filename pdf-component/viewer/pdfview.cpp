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

    QRect iRect = viewRect.toAlignedRect();

    return page->renderToImage(scale, scale,
         iRect.left(), iRect.top(), iRect.width(), iRect.height());
  }
};

PdfView::PdfView(QWidget *parent) 
    : QLabel(parent) {
  pageIndex = 0;
  pageIndexEntry = 0;
  doc = 0;

  scalingStrategies << new FitPageScaler() << new TextScaler();
  scalerIndex = 0;

  setFocusPolicy(Qt::StrongFocus);
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
    QFont font;
    QFontMetrics metrics(font);

    const int pad = 8;
    const int lineHeight = metrics.lineSpacing();
    const int boxHeight = (lineHeight + pad) * 2;
    const QString prompt("Go to page:");
    const int promptWidth = metrics.width(prompt);
    const int boxWidth = promptWidth + (pad * 2);

    QSize canvasSize = size();
    QPoint boxCenter(canvasSize.width() / 2, canvasSize.height() / 2);
    QRect boxRect(boxCenter.x() - (boxWidth / 2), boxCenter.y() - (boxHeight / 2), boxWidth, boxHeight);
    qDebug() << boxRect;

    QPainter painter(this);

    painter.setBrush(QBrush(QColor(0xFF, 0xFF, 0xFF/*, 0xAA*/)));
    painter.drawRoundedRect(boxRect, 10, 10);

    painter.setFont(font);
    
    painter.drawText(boxCenter.x() - (promptWidth / 2), boxCenter.y(), prompt);
    QString entry = QString("%1_").arg(pageIndexEntry);
    painter.drawText(boxCenter.x() - metrics.width(entry) / 2, boxCenter.y() + lineHeight, entry);
  }
}

bool PdfView::setDocument(const QString &path) {
  Poppler::Document *oldDoc = doc;

  doc = Poppler::Document::load(path);
  delete oldDoc;

  if (doc) {
    doc->setRenderHint(Poppler::Document::Antialiasing);
    doc->setRenderHint(Poppler::Document::TextAntialiasing);
    emit morePages(doc->numPages());
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
    int entry = (pageIndexEntry * 10) + (event->key() - Qt::Key_0);
    if (entry < doc->numPages()) pageIndexEntry = entry;
    update(0, 150, 600, 350);
  } else if (event->key() == Qt::Key_Escape) {
    if (pageIndexEntry > 0) {
      pageIndexEntry /= 10;
      update(0, 150, 600, 350);
    } else {
      emit back();
    }
  } else if (event->key() == Qt::Key_Return && pageIndexEntry > 0) {
    showPage(pageIndexEntry - 1);
    pageIndexEntry = 0;
  } else {
    QLabel::keyPressEvent(event);
  }
}
