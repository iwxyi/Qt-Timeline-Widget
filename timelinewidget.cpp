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
    bucket->setVerticalIndex(count()-1); // 已经添加了，下标索引要-1
    setItemWidget(item, bucket);
    item->setSizeHint(bucket->getSuitableSize());

    // 设置item的尺寸
    connect(bucket, &TimeBucket::signalSizeHintChanged, this, [=](QSize size){
        item->setSizeHint(size);
    });

    updateUI();
}

TimeBucket *TimelineWidget::createItemWidget(QString time, QStringList texts)
{
    TimeBucket* bucket = new TimeBucket(this);
    bucket->setTime(time);
    bucket->setText(texts);
    return bucket;
}

void TimelineWidget::updateUI()
{
    QString style = "#TimelineTimeLabel { background:white; border: 1px solid orange; border-radius: 5px; padding: 10px; }"
            "#TimelineTextLabel { background:white; border: 1px solid blue; border-radius: 5px; padding:10px; }";
    setStyleSheet(style);
}
