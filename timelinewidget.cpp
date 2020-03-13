#include "timelinewidget.h"

TimelineWidget::TimelineWidget(QWidget *parent) : QListWidget(parent)
{

}

void TimelineWidget::addItem(QString time, QString text)
{
    QListWidgetItem* item = new QListWidgetItem(this);
    QWidget* widget = createItemWidget(time, text);
    setItemWidget(item, widget);
}

TimeBucket* TimelineWidget::createItemWidget(QString time, QString text)
{
    TimeBucket* bucket = new TimeBucket(this);
    bucket->setTime(time);
    bucket->setText(text);
    return bucket;
}

TimeBucket *TimelineWidget::createItemWidget(QString time, QStringList texts)
{
    TimeBucket* bucket = new TimeBucket(this);
    bucket->setTime(time);
    bucket->setText(texts);
    return bucket;
}
