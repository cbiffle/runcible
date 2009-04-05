#include "choiceview.h"

#include <QKeyEvent>
#include <QPaintEvent>
#include <QPainter>

Choice::Choice(const QString &title, const QString &id, const QStringList &params)
    : _id(id), _title(title), _params(params) {
}

const QString &Choice::id() const {
  return _id;
}

const QString &Choice::title() const {
  return _title;
}

const QStringList &Choice::params() const {
  return _params;
}

ChoiceView::ChoiceView(QWidget *parent)
    : QWidget(parent),
      _offset(0) {

  setFocusPolicy(Qt::StrongFocus);

  QFont myFont;
  myFont.setPointSize(20);
  setFont(myFont);
}

ChoiceView::~ChoiceView() { }

static const int arrowHeight = 8;

static char _hotkeys[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
void ChoiceView::paintEvent(QPaintEvent *event) {
  QFontMetrics metrics(font());
  QPainter painter(this);

  int y = arrowHeight + 2;
  const int baseline = metrics.leading() + metrics.ascent() + 1;
  const int w = width(), h = height() - (y * 2);
  const int yInc = h / 10;
  const int labelWidth = w - yInc - 10;
  const int hotkeyCenter = w - (yInc / 2);

  // Top arrow
  const int center = w / 2;
  painter.eraseRect(0, 0, w, y);
  if (_offset > 0) {
    painter.drawLine(center, 0, center - arrowHeight, arrowHeight);
    painter.drawLine(center, 0, center + arrowHeight, arrowHeight);
    painter.drawLine(center - arrowHeight, arrowHeight, center + arrowHeight, arrowHeight);
  }
  
  // Bottom arrow
  painter.eraseRect(0, height() - y, w, y);
  if (_offset + 10 < _choices.size()) {
    painter.drawLine(center, height() - 1, center - arrowHeight, height() - arrowHeight - 1);
    painter.drawLine(center, height() - 1, center + arrowHeight, height() - arrowHeight - 1);
    painter.drawLine(center - arrowHeight, height() - arrowHeight - 1,
        center + arrowHeight, height() - arrowHeight - 1);
  }

  QLinearGradient fade(QPointF(0,0), QPointF(labelWidth, 0));
  fade.setColorAt(0, Qt::black);
  fade.setColorAt(0.95, Qt::black);
  fade.setColorAt(1.0, QColor(0xAA, 0xAA, 0xAA));
 
  QTextOption noWrap;
  noWrap.setWrapMode(QTextOption::NoWrap);

  // Choice area
  for (int row = 0; row < NUM_CHOICES; row++) {
    const int item = row + _offset;
    painter.eraseRect(0, y, w, yInc);
    if (item < _choices.size()) {
      const Choice &c = _choices[item];
      if (metrics.width(c.title()) > labelWidth) {
        painter.setPen(QPen(QBrush(fade), 1));
      } else {
        painter.setPen(QPen(Qt::black));
      }

      painter.drawText(QRectF(0, y, labelWidth, yInc), c.title(), noWrap);

      painter.save();
      if (row > 0) {
        painter.setPen(QColor(0xAA, 0xAA, 0xAA));
        painter.drawLine(50, y - 10, w - 50, y - 10);
      }

      painter.setPen(QColor(0x55, 0x55, 0x55));
      painter.drawRoundedRect(labelWidth + 10, y + metrics.leading(), yInc - 1, metrics.height(), 4, 4);
      painter.drawText(hotkeyCenter - (metrics.width(_hotkeys[row]) / 2), y + baseline,
          QString(1, _hotkeys[row]));

      painter.restore();
    }
    y += yInc;
  }
}

void ChoiceView::keyPressEvent(QKeyEvent *event) {
  if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
    int chosen = event->key() - Qt::Key_0 + _offset - 1;
    if (chosen < _offset) chosen += 10;
    if (chosen < _choices.size()) {
      choose(chosen);
      return;
    }
  } else if (event->key() == Qt::Key_Escape) {
    goBack();
    return;
  } else if (event->key() == Qt::Key_Down) {
    pageDown();
    return;
  } else if (event->key() == Qt::Key_Up) {
    pageUp();
    return;
  }
  QWidget::keyPressEvent(event);
}

void ChoiceView::choose(int chosen) {
  emit choiceMade(_choices[chosen]);
}

void ChoiceView::goBack() {
  emit back();
}

void ChoiceView::setChoices(const QList<Choice> choices) {
  _choices = choices;
  if (_offset >= _choices.size()) {
    // Integer division floors here.
    _offset = (_choices.size() / 10) * 10;
  }
  emit morePages(_choices.size() / 10);
  refreshLabels();
}

void ChoiceView::pageDown() {
  if (_offset < _choices.size() - 10) {
    _offset += 10;
    refreshLabels();
  }
}

void ChoiceView::pageUp() {
  if (_offset >= 10) {
    _offset -= 10;
    refreshLabels();
  }
}

void ChoiceView::refreshLabels() {
  update();
  emit switchedToPage(_offset / 10);
}

bool operator<(Choice a, Choice b) {
  return a.title().compare(b.title(), Qt::CaseInsensitive) < 0;
}
