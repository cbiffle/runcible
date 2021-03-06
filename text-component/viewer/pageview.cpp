#include "pageview.h"

#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include <QTextDocument>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QUrl>
#include <QSettings>
#include <QVariant>

PageView::PageView(QWidget *parent)
    : RDocView(parent),
      _doc(0) {
}

PageView::~PageView() {}

void PageView::setDocument(const QString &path, QTextDocument *doc) {
  reset();
  _doc = doc;
  if (_doc != 0) {
    QSettings s("runcible", "text-component");
    int ptSize = s.value("font-size", 12).toInt();
    QFont font(_doc->defaultFont());
    font.setPointSize(ptSize);
    _doc->setDefaultFont(font);
    _doc->setPageSize(size());
    (void) _doc->documentLayout();

    contentsChanged(QUrl::fromLocalFile(path));
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
    if (_doc->pageSize() != size()) {
      _doc->setPageSize(size());
      emit pageCountChanged(_doc->pageCount());
    }
  }
}

int PageView::pageCount() {
  if (_doc == 0) return 0;
  return _doc->pageCount();
}

static void saveFontSize(int pts) {
  QSettings s("runcible", "text-component");
  s.setValue("font-size", QVariant(pts));
}

void PageView::zoomIn() {
  if (_doc == 0) return;

  QFont font(_doc->defaultFont());
  font.setPointSize(qMin(font.pointSize() + 2, 20));
  _doc->setDefaultFont(font);
  emit pageCountChanged(pageCount());
  update();

  saveFontSize(font.pointSize());
}

void PageView::zoomOut() {
  if (_doc == 0) return;

  QFont font(_doc->defaultFont());
  font.setPointSize(qMax(font.pointSize() - 2, 6));
  _doc->setDefaultFont(font);
  emit pageCountChanged(pageCount());
  update();

  saveFontSize(font.pointSize());
}
