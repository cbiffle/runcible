#include "choiceview.h"

#include <QKeyEvent>
#include <QGridLayout>
#include <QLabel>

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
      _layout(new QGridLayout),
      _offset(0) {

  setFocusPolicy(Qt::StrongFocus);

  _layout->setColumnStretch(0, 1);

  QFont labelFont("Liberation Serif", 20);

  setUpdatesEnabled(false);
  for (int i = 0; i < NUM_CHOICES; i++) {
    QLabel *label = new QLabel("");
    label->setFont(labelFont);
    _layout->addWidget(label, i, 0);
    _labels << label;

    QLabel *hotkey = new QLabel(QString::number((i + 1) % 10));
    hotkey->setFont(QFont("Liberation Serif", 20, QFont::Bold));
    hotkey->setFrameShape(QFrame::Box);
    hotkey->setLineWidth(1);
    _layout->addWidget(hotkey, i, 1);
    _hotkeys << hotkey;
  }
  setLayout(_layout);
  setUpdatesEnabled(true);
}

ChoiceView::~ChoiceView() {
  delete _layout;
  qDeleteAll(_labels);
  qDeleteAll(_hotkeys);
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
  setUpdatesEnabled(false);
  int cs = _choices.size();
  QFontMetrics metrics(_labels[0]->font());

  for (int i = 0; i < NUM_CHOICES; i++) {
    if (i + _offset < cs) {
      const QString &title = _choices[i + _offset].title();
      QString elided(metrics.elidedText(title, Qt::ElideMiddle, _labels[i]->width()));
      _labels[i]->setText(elided);
      _hotkeys[i]->show();
    } else {
      _labels[i]->setText("");
      _hotkeys[i]->hide();
    }
  }

  emit switchedToPage(_offset / 10);
  setUpdatesEnabled(true);
}

bool operator<(Choice a, Choice b) {
  return a.title().compare(b.title(), Qt::CaseInsensitive) < 0;
}
