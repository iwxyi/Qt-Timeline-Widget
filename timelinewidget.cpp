#include "timelinewidget.h"

TimelineWidget::TimelineWidget(QWidget *parent) : QListWidget(parent)
{

}

void TimelineWidget::addItem(QString time, QString text)
{
    addItem(time, QStringList{text});
}

void TimelineWidget::addItem(QString time, QStringList texts)
{
    QListWidgetItem* item = new QListWidgetItem(this);
    TimeBucket* bucket = createItemWidget(time, texts);
    buckets.append(bucket);
    setItemWidget(item, bucket);
    item->setSizeHint(bucket->getSuitableSize());

    // 设置item的尺寸
    connect(bucket, &TimeBucket::signalSizeHintChanged, this, [=](QSize size){
        item->setSizeHint(size);
        qDebug() << "set item size" << size;
    });
}

TimeBucket *TimelineWidget::createItemWidget(QString time, QStringList texts)
{
    TimeBucket* bucket = new TimeBucket(this);
    bucket->setTime(time);
    bucket->setText(texts);
    return bucket;
}
