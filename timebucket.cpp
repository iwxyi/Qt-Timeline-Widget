#include "timebucket.h"

TimeBucket::TimeBucket(QWidget *parent) : QWidget(parent)
{
    initView();
}

void TimeBucket::initView()
{
    hlayout = new QHBoxLayout(this);
    time_widget = new TimelineTimeLabel(this);

    hlayout->addWidget(time_widget);

    setLayout(hlayout);
}

void TimeBucket::setTime(QString time)
{
    time_widget->setText(time);
}

void TimeBucket::setText(int index, QString text)
{
    if (index < 0 || index > text_widgets.size()) // 错误
        return ;
    if (index == text_widgets.size()) // 加一个
        return addTextWidget(text);

    // 设置内容
    text_widgets.at(index)->setText(text);
}

void TimeBucket::setText(QString text)
{
    this->clearText();
    addTextWidget(text);
}

void TimeBucket::setText(QStringList texts)
{
    this->clearText();
    foreach (QString text, texts)
    {
        addTextWidget(text);
    }
}

void TimeBucket::addTextWidget(QString text)
{
    TimelineTextLabel* label = new TimelineTextLabel(this);
    label->setText(text);
    text_widgets.append(label);
    hlayout->addWidget(label);
}

void TimeBucket::clearText()
{
    foreach (TimelineTextLabel* label, text_widgets) {
        label->deleteLater();
    }
    text_widgets.clear();
}

void TimeBucket::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    // 每个控件之前画线条
}

void TimeBucket::addOneLabel(QString text)
{

}
