#include "choiceview.h"

#include <QKeyEvent>
#include <QGridLayout>
#include <QLabel>

Choice::Choice(const QString &title, const QString &id, const QStringList &params)
    : _id(id), _title(title), _params(params) { }

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

  _layout->setColumnStretch(0, 1);

  for (int i = 0; i < NUM_CHOICES; i++) {
    QLabel *label = new QLabel("HI");
    label->setFont(QFont("sans", 20));
    label->setMaximumWidth(16777215);
    _layout->addWidget(label, i, 0);
    _labels << label;

    QLabel *hotkey = new QLabel(QString::number((i + 1) % 10));
    hotkey->setFont(QFont("sans", 20, QFont::Bold));
    hotkey->setFrameShape(QFrame::Box);
    hotkey->setFrameShadow(QFrame::Raised);
    hotkey->setLineWidth(3);
    _layout->addWidget(hotkey, i, 1);
    _hotkeys << hotkey;
  }
  setLayout(_layout);
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
  qDebug() << _choices[chosen].id();
  emit choiceMade(_choices[chosen]);
}

void ChoiceView::goBack() {
  qDebug() << "BACK";
  emit back();
}

void ChoiceView::setChoices(const QList<Choice> choices) {
  _choices = choices;
  if (_offset >= _choices.size()) {
    // Integer division floors here.
    _offset = (_choices.size() / 10) * 10;
  }
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
  int cs = _choices.size();
  for (int i = 0; i < NUM_CHOICES; i++) {
    if (i + _offset < cs) {
      _labels[i]->setText(_choices[i + _offset].title());
      _hotkeys[i]->show();
    } else {
      _labels[i]->setText("");
      _hotkeys[i]->hide();
    }
  }
}
