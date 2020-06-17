#ifndef BACKPACKWIDGET_H
#define BACKPACKWIDGET_H

#include <QListWidget>
#include <QVBoxLayout>
#include "timelinewidget.h"

class BackpackWidget : public QWidget
{
    Q_OBJECT
public:
    BackpackWidget(TimelineWidget* timeline, QWidget *parent = nullptr);

    struct TimeThing {
        QString name;   // 物品的名字
        QString value;  // 物品的值（只有可修改项才有）
        int time_index; // 对应时间轴的下标
    };

private:
    void initView();

signals:

public slots:
    void refreshTimeline();

private:
    QListWidget* list_widget;
    TimelineWidget* timeline;
    QList<TimeThing> things;
};

#endif // BACKPACKWIDGET_H
