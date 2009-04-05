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
      _pageIndex(0) {
  setFocusPolicy(Qt::StrongFocus);
}

PageView::~PageView() {}

void PageView::setDocument(QTextDocument *doc) {
  _doc = doc;
  _pageIndex = 0;
  //update();
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

void PageView::paintEvent(QPaintEvent *event) {
  if (_doc == 0) return;

  QRectF pageRect(0, 0, width(), height());
  _doc->setPageSize(pageRect.size());

  (void) _doc->documentLayout();

  QPainter p(this);
  renderPage(_pageIndex, &p, _doc, pageRect);
}

void PageView::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Up:
      pageUp();
      return;

    case Qt::Key_Down:
      pageDown();
      return;

  }
  QWidget::keyPressEvent(event);
}

void PageView::pageUp() {
  if (_doc != 0) {
    _pageIndex = qMax(0, _pageIndex - 1);
    qDebug() << _pageIndex;
    update();
  }
}

void PageView::pageDown() {
  if (_doc != 0) {
    _pageIndex = qMin(_doc->pageCount() - 1, _pageIndex + 1);
    qDebug() << _pageIndex;
    update();
  }
}
