#include "backpackwidget.h"

BackpackWidget::BackpackWidget(QWidget *parent) : QWidget(parent)
{
    initView();
}

void BackpackWidget::setTimeline(TimelineWidget *timeline)
{
    this->timeline = timeline;

    connect(timeline, SIGNAL(selectedItemsChanged()), this, SLOT(autoRefreshTimeline()));
}

void BackpackWidget::initView()
{
    bps_combo = new QComboBox(this);
    list_widget = new QListWidget(this);
    refresh_btn = new QPushButton("刷新", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(bps_combo);
    layout->addWidget(list_widget);
    layout->addWidget(refresh_btn);
    setLayout(layout);

    connect(bps_combo, &QComboBox::currentTextChanged, this, [=](QString name){
        if (bps_combo->count() == 0)
            return ;
        current_backpack = name;
        refreshThings();
    });
    connect(list_widget, &QListWidget::itemActivated, this, [=](QListWidgetItem *item){
        int row = list_widget->row(item);
        int index = backpacks.value(current_backpack).at(row).time_index;
        is_focusing_item = true;
        timeline->setCurrentItem(index);
        is_focusing_item = false;
    });
    connect(refresh_btn, SIGNAL(clicked()), this, SLOT(refreshTimeline()));
}

void BackpackWidget::keyPressEvent(QKeyEvent *event)
{
    auto key = event->key();
    auto modifier = event->modifiers();

    if (modifier == Qt::NoModifier && (key == Qt::Key_F5 || key == Qt::Key_Refresh))
    {
        refreshTimeline();
        return ;
    }

    QWidget::keyPressEvent(event);
}

void BackpackWidget::autoRefreshTimeline()
{
    if (auto_refresh && !is_focusing_item)
        refreshTimeline();
}

/**
 * 根据当前点击的那些行，获取对应的背包
 */
void BackpackWidget::refreshTimeline()
{
    backpacks.clear();

    // ==== 获取选中行 ====
    auto indexes = timeline->selectedIndexes();
    QList<int> watched_indexes;

    // 若是没选，则遍历全部
    if (indexes.size() == 0)
    {
        int size = timeline->count();
        for (int i = 0; i < size; i++)
            watched_indexes.append(i);
    }
    // 若是单选，则选中当前行及上面
    else if (indexes.size() == 1)
    {
        int size = indexes.first();
        for (int i = 0; i <= size; i++)
            watched_indexes.append(i);
    }
    // 若是多选，则只查看选中的那些行
    else
    {
        watched_indexes = indexes;
    }

    // ==== 构建背包正则 ====
    /* 格式：
     * +物品
     * -物品
     * *等级=3级
     * @背包-物品
     * @仓库+物品
     * @主角*等级=3
     */
    QRegularExpression addRe("^(@(.*))?\\+(.+)");
    QRegularExpression delRe("^(@(.*))?-(.+)");
    QRegularExpression modRe("^(@(.*))?\\*(.+?)=(.*)");
    QRegularExpression modAddRe("^(@(.*))?\\*(.+?)\\+(.*)");
    QRegularExpression modCutRe("^(@(.*))?\\*(.+?)-(.*)");
    auto getBackpack = [&](QString name) {
        if (!backpacks.contains(name))
            backpacks.insert(name, QList<TimeThing>{});
        return &backpacks[name];
    };

    // ==== 顺序遍历背包 ====
    for (int i = 0; i < watched_indexes.size(); i++)
    {
        auto bucket = timeline->at(watched_indexes.at(i));
        auto time = bucket->getTime();
        auto texts = bucket->getTexts();

        foreach (auto text, texts)
        {
            QRegularExpressionMatch match;
            // +物品
            if (text.indexOf(addRe, 0, &match) > -1)
            {
                auto rsts = match.capturedTexts();
                QString bp = rsts.at(2); // 背包名字（可空）
                QString name = rsts.at(3); // 物品名字
                TimeThing thing;
                thing.time_index = watched_indexes.at(i);
                thing.name = name;
                getBackpack(bp)->append(thing);
            }

            // -物品
            else if (text.indexOf(delRe, 0, &match) > -1)
            {
                auto rsts = match.capturedTexts();
                QString bp = rsts.at(2); // 背包名字（可空）
                QString name = rsts.at(3); // 物品名字
                auto things = getBackpack(bp);
                for (int j = 0; j < things->size(); j++)
                {
                    if (things->at(j).name == name)
                    {
                        things->removeAt(j);
                        break;
                    }
                }
            }

            // *物品=属性

            // *物品+数值  *物品-数值

        }
    }

    // ==== 内容显示至视图 ====
    bps_combo->clear();
    bps_combo->addItems(backpacks.keys());
    bps_combo->setCurrentText(current_backpack);

    /*QString max_name = "";
    int max_count = 0;
    for (auto i = backpacks.begin(); i != backpacks.end(); i++)
    {
        if (i.value().size() > max_count)
        {
            max_count = i.value().size();
            max_name = i.key();
        }
    }
    current_backpack = max_name;*/

}

void BackpackWidget::refreshThings()
{
    list_widget->clear();
    auto things = backpacks.value(current_backpack);
    foreach (auto thing, things) {
        QString text = thing.name;
        if (!thing.value.isEmpty())
            text += "：" + thing.value;
        QListWidgetItem* item = new QListWidgetItem(text, list_widget);
    }
}
