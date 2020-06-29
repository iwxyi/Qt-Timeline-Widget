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
    layout->setMargin(0);
    layout->addWidget(bps_combo);
    layout->addWidget(list_widget);
    layout->addWidget(refresh_btn);
    setLayout(layout);

    /*connect(bps_combo, &QComboBox::currentTextChanged, this, [=](QString name){
        if (bps_combo->count() == 0)
            return ;
        current_backpack = name;
        refreshThings();
    });*/
    connect(bps_combo, SIGNAL(activated(const QString&)), this, SLOT(slotComboChanged(const QString&)));
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
    QRegularExpression addDelRe("^(@(.*))?([\\+\\-])(.+)"); // 物品加减
    QRegularExpression attrRe("^(@(.*))?\\*(.+?)([\\+\\-=])(.*)"); // 属性加减
    QRegularExpression numUnitRe("^(-?\\d{1,8})\\s?(.*)$"); // 带单位的数字
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
            // @背包+物品  -物品
            if (text.indexOf(addDelRe, 0, &match) > -1)
            {
                auto rsts = match.capturedTexts();
                QString bp = rsts.at(2); // 背包名字（可空）
                QString op = rsts.at(3);
                QString name = rsts.at(4); // 物品名字
                if (op == "+") // +物品
                {
                    TimeThing thing;
                    thing.name = name;
                    thing.time_index = watched_indexes.at(i);
                    getBackpack(bp)->append(thing);
                }
                else if(op == "-") // -物品
                {
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
            }

            // @背包*物品=属性    @背包*物品+数值  *物品-数值
            else if (text.indexOf(attrRe, 0, &match) > -1)
            {
                auto rsts = match.capturedTexts();
                QString bp = rsts.at(2);
                QString name = rsts.at(3);
                QString op = rsts.at(4);
                QString value = rsts.at(5);

                // 查找或创建
                auto things = getBackpack(bp);
                TimeThing* thing = nullptr;
                for (int j = 0; j < things->size(); j++)
                {
                    if (things->at(j).name == name)
                    {
                        thing = &(*things)[j];
                        break;
                    }
                }
                if (thing == nullptr) // 不存在这个物品，创建
                {
                    TimeThing newThing;
                    newThing.name = name;
                    newThing.time_index = watched_indexes.at(i);
                    things->append(newThing);
                    thing = &(*things)[things->size()-1];
                }

                // 修改属性
                if (op == "=" || (thing->value.isEmpty() && op == "+")) // （空）物品=属性
                {
                    thing->value = value;
                }
                else if (thing->value.isEmpty() && op == "-") // 空物品，负数
                {
                    thing->value = "-" + value;
                }
                else if (op == "+" || op == "-")
                {
                    QRegularExpressionMatch match2;
                    if (thing->value.indexOf(numUnitRe, 0, &match2) == -1)
                        continue;
                    auto texts = match2.capturedTexts();
                    QString num = texts.at(1);  // 数值
                    QString unit = texts.at(2); // 单位
                    if (num.isEmpty())
                        num = "0";

                    if (value.indexOf(numUnitRe, 0, &match2) == -1)
                        continue;
                    texts = match2.capturedTexts();
                    QString num2 = texts.at(1);  // 数值
                    QString unit2 = texts.at(2); // 单位
                    if (num2.isEmpty())
                        continue;

                    int iNum = num.toLongLong();
                    int iNum2 = num2.toLongLong();

                    // 要确保单位一样
                    if (unit != unit2)
                    {
                        if (unit2.endsWith(unit))
                        {
                            while (!unit2.isEmpty())
                            {
                                QString ch = unit2.mid(0, 1);
                                if (ch == "十")
                                    iNum2 *= 10;
                                else if (ch == "百")
                                    iNum2 *= 100;
                                else if (ch == "千")
                                    iNum2 *= 1000;
                                else if (ch == "万")
                                    iNum2 *= 10000;
                                else if (ch == "亿")
                                    iNum2 *= 100000000;
                                else if (ch == "兆")
                                    iNum2 *= 10000000000;
                                else
                                    break;
                                unit2 = unit2.right(unit2.length()-1);
                                if (unit2 == unit) // 已经转换完成
                                    break;
                            }
                            if (unit2 != unit && !unit2.isEmpty()) // 未知转换
                                continue;
                        }
                        else if (!unit.isEmpty() && unit2.isEmpty())
                            unit2 = unit; // 默认单位
                        else // 无法转换
                            continue;
                    }

                    if (op == "+") // 物品+数值
                    {
                        iNum += iNum2;
                    }
                    else if (op == "-") // 物品-数值
                    {
                        iNum -= iNum2;
                    }
                    thing->value = QString("%1%2").arg(iNum).arg(unit);
                }
            }
        }
    }

    // ==== 内容显示至视图 ====
    bps_combo->clear();
    bps_combo->addItems(backpacks.keys()); // 会触发currentChanged信号
    if (backpacks.contains(current_backpack))
        bps_combo->setCurrentText(current_backpack);
    else
        bps_combo->setCurrentText(current_backpack = "");
    refreshThings();

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
        new QListWidgetItem(text, list_widget);
    }
}

void BackpackWidget::slotComboChanged(const QString &name)
{
    if (bps_combo->count() == 0)
        return ;
    current_backpack = name;
    refreshThings();
}
