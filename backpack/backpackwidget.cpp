#include "backpackwidget.h"

BackpackWidget::BackpackWidget(TimelineWidget *timeline, QWidget *parent) : QWidget(parent), timeline(timeline)
{
    initView();
}

void BackpackWidget::initView()
{
    list_widget = new QListWidget(this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(list_widget);
    setLayout(layout);
}

/**
 * 根据当前点击的那些行，获取对应的背包
 */
void BackpackWidget::refreshTimeline()
{
    things.clear();

    // 获取当前行
    // 若是没选，则遍历全部

    // 若是单选，则选中当前行及上面

    // 若是多选，则只查看选中的那些行


    // 内容显示至视图
    list_widget->clear();
    foreach (auto thing, things) {
        QString text = thing.name;
        if (!thing.value.isEmpty())
            text += "：" + thing.value;
        QListWidgetItem* item = new QListWidgetItem(text, list_widget);
    }
}
