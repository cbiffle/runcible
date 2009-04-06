#include "rdocview.h"

#include <QPainter>
#include <QPaintEvent>
#include <QKeyEvent>

RDocView::RDocView(QWidget *parent)
    : QWidget(parent),
      _pageIndex(0),
      _pageNumberEntry(0) {
  setFocusPolicy(Qt::StrongFocus);
}

RDocView::~RDocView() {}

static void drawGoToDialog(QPainter &p, int number, const QRect &boxRect) {
  QFontMetrics metrics(p.font());

  const int pad = 8;
  const int lineHeight = metrics.lineSpacing();
  const QString prompt("Go to page:");
  const int promptWidth = metrics.width(prompt);

  const QPoint center = boxRect.center();

  p.save();
  p.setBrush(QBrush(QColor(0xFF, 0xFF, 0xFF, 0xAA)));
  p.drawRoundedRect(boxRect, 10, 10);

  p.drawText(center.x() - (promptWidth / 2), center.y(), prompt);
  QString entry = QString("%1_").arg(number);
  p.drawText(boxRect.left() + pad, center.y() + lineHeight, entry);
  p.restore();
}

void RDocView::paintEvent(QPaintEvent *event) {
  renderPage(_pageIndex);

  if (_pageNumberEntry > 0) {
    QPainter p(this);
    drawGoToDialog(p, _pageNumberEntry, _boxRect);
  }
}

void RDocView::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Up:
      pageUp();
      return;

    case Qt::Key_Down:
      pageDown();
      return;

    case Qt::Key_Escape:
      if (_pageNumberEntry > 0) {
        if (event->modifiers() & Qt::AltModifier) {
          _pageNumberEntry = 0;
        } else {
          _pageNumberEntry /= 10;
        }
        update(_boxRect.adjusted(0,0,1,1));
        return;
      }
      break;

    case Qt::Key_Return:
      if (_pageNumberEntry > 0) {
        goToPage(_pageNumberEntry - 1);
      }
      break;

    case Qt::Key_Plus:
      zoomIn();
      break;

    case Qt::Key_Minus:
      zoomOut();
      break;
  }

  if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
    _pageNumberEntry = _pageNumberEntry * 10 + (event->key() - Qt::Key_0);
    update(_boxRect.adjusted(0,0,1,1));
    return;
  }

  QWidget::keyPressEvent(event);
}

void RDocView::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);

  QRectF pageRect(0, 0, width(), height());
 
  QPoint center = pageRect.center().toPoint();
  QFontMetrics metrics(font());

  const int pad = 8;
  const int lineHeight = metrics.lineSpacing();
  const int boxHeight = (lineHeight + pad) * 2;
  const QString prompt("Go to page:");
  const int promptWidth = metrics.width(prompt);
  const int boxWidth = promptWidth + pad * 2;

  _boxRect = QRect( center.x() - (boxWidth / 2), center.y() - (boxHeight / 2), boxWidth, boxHeight );
}

void RDocView::goToPage(int page) {
  _pageNumberEntry = 0;
  if (page != _pageIndex) {
    _pageIndex = qBound(0, page, pageCount() - 1);
    emit pageChanged(_pageIndex);
    update();
  }
}
void RDocView::pageUp() {
  goToPage(_pageIndex - 1);
}

void RDocView::pageDown() {
  goToPage(_pageIndex + 1);
}

void RDocView::reset() {
  _pageIndex = 0;
  _pageNumberEntry = 0;
}

int RDocView::pageIndex() {
  return _pageIndex;
}

void RDocView::zoomIn() { }
void RDocView::zoomOut() { }
