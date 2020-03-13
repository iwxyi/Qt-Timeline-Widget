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
    hlayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
}

void TimeBucket::setTime(QString time)
{
    time_widget->setText(time);
    time_widget->adjustSize();
    time_widget->setScaledContents(true);
}

void TimeBucket::setText(int index, QString text)
{
    if (index < 0 || index > text_widgets.size()) // 错误
        return ;
    if (index == text_widgets.size()) // 加一个
        return addTextWidget(text);

    // 设置内容
    text_widgets.at(index)->setText(text);

    emit signalSizeHintChanged(getSuitableSize());
}

void TimeBucket::setText(QString text)
{
    this->clearText();
    addTextWidget(text);

    emit signalSizeHintChanged(getSuitableSize());
}

void TimeBucket::setText(QStringList texts)
{
    this->clearText();
    foreach (QString text, texts)
    {
        addTextWidget(text);
    }

    emit signalSizeHintChanged(getSuitableSize());
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

void TimeBucket::setTimeLabelWidth(int w)
{
    time_widget->setFixedWidth(w);
}

/**
 * 根据里面的内容，获取对应合适的宽度和高度
 */
QSize TimeBucket::getSuitableSize()
{
    int sw = TIMELINE_BUBBLES_SPACING + time_widget->width();
    int sh = 0;
    for (int i = 0; i < text_widgets.size(); ++i)
    {
        if (sh < text_widgets.at(i)->height())
            sh = text_widgets.at(i)->height();
        sw += TIMELINE_BUBBLES_SPACING + text_widgets.at(i)->width();
    }
    sw += hlayout->margin() * 2;
    sh += hlayout->margin() * 2;
    return QSize(sw, sh);
}

void TimeBucket::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    // 每个控件之前画线条
}
