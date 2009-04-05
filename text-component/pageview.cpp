#include "pageview.h"

#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include <QTextDocument>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QDebug>

PageView::PageView(QWidget *parent)
    : QWidget(parent),
      _doc(0),
      _pageIndex(0),
      _pageNumberEntry(0) {
  setFocusPolicy(Qt::StrongFocus);
}

PageView::~PageView() {}

void PageView::setDocument(QTextDocument *doc) {
  _doc = doc;
  _pageIndex = 0;
  _pageNumberEntry = 0;
  if (_doc != 0) {
    QRectF pageRect(0, 0, width(), height());
    _doc->setPageSize(pageRect.size());
  }

  (void) _doc->documentLayout();

  emit pageCountChanged(_doc->pageCount());
}

static void renderPage(int index, QPainter *p, const QTextDocument *doc, const QRectF &body) {
  p->save();

  p->translate(body.left(), body.top() - (index * body.height()));

  QRectF view(0, index * body.height(), body.width(), body.height());

  QAbstractTextDocumentLayout *layout = doc->documentLayout();
  QAbstractTextDocumentLayout::PaintContext ctx;

  p->setClipRect(view);
  ctx.clip = view;

  layout->draw(p, ctx);

  p->restore();
}

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

void PageView::paintEvent(QPaintEvent *event) {
  if (_doc == 0) return;

  QRectF pageRect(0, 0, width(), height());

  QPainter p(this);
  renderPage(_pageIndex, &p, _doc, pageRect);
  if (_pageNumberEntry > 0) {
    drawGoToDialog(p, _pageNumberEntry, _boxRect);
  }
}

void PageView::keyPressEvent(QKeyEvent *event) {
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
  }

  if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
    _pageNumberEntry = _pageNumberEntry * 10 + (event->key() - Qt::Key_0);
    update(_boxRect.adjusted(0,0,1,1));
    return;
  }

  QWidget::keyPressEvent(event);
}

void PageView::resizeEvent(QResizeEvent *event) {
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

  if (_doc != 0) {
    _doc->setPageSize(pageRect.size());
  }
}

void PageView::goToPage(int page) {
  _pageNumberEntry = 0;
  if (_doc != 0 && page != _pageIndex) {
    _pageIndex = qBound(0, page, _doc->pageCount() - 1);
    emit pageChanged(_pageIndex);
    update();
  }
}
void PageView::pageUp() {
  goToPage(_pageIndex - 1);
}

void PageView::pageDown() {
  goToPage(_pageIndex + 1);
}
