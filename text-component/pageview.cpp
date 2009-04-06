#include "pageview.h"

#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include <QTextDocument>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QDebug>

PageView::PageView(QWidget *parent)
    : RDocView(parent),
      _doc(0) {
}

PageView::~PageView() {}

void PageView::setDocument(QTextDocument *doc) {
  reset();
  _doc = doc;
  if (_doc != 0) {
    QRectF pageRect(0, 0, width(), height());
    _doc->setPageSize(pageRect.size());
    (void) _doc->documentLayout();

    emit pageCountChanged(_doc->pageCount());
    update();
  }
}

void PageView::renderPage(int index) {
  if (_doc == 0) return;

  QRectF body(0, 0, width(), height());
  QPainter p(this);

  p.translate(body.left(), body.top() - (index * body.height()));

  QRectF view(0, index * body.height(), body.width(), body.height());

  QAbstractTextDocumentLayout *layout = _doc->documentLayout();
  QAbstractTextDocumentLayout::PaintContext ctx;

  p.setClipRect(view);
  ctx.clip = view;

  layout->draw(&p, ctx);
}

void PageView::resizeEvent(QResizeEvent *event) {
  RDocView::resizeEvent(event);

  if (_doc != 0) {
    _doc->setPageSize(size());
    emit pageCountChanged(_doc->pageCount());
  }
}

int PageView::pageCount() {
  if (_doc == 0) return 0;
  return _doc->pageCount();
}

void PageView::zoomIn() {
  if (_doc == 0) return;

  QFont font(_doc->defaultFont());
  font.setPointSize(qMin(font.pointSize() + 2, 20));
  _doc->setDefaultFont(font);
  emit pageCountChanged(pageCount());
  update();
}

void PageView::zoomOut() {
  if (_doc == 0) return;

  QFont font(_doc->defaultFont());
  font.setPointSize(qMax(font.pointSize() - 2, 6));
  _doc->setDefaultFont(font);
  emit pageCountChanged(pageCount());
  update();
}
