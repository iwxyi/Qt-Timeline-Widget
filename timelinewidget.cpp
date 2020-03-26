#include "timelinewidget.h"

TimelineWidget::TimelineWidget(QWidget *parent) : QWidget(parent)
{
    setAcceptDrops(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested (const QPoint&)),this,SLOT(slotMenuShowed(const QPoint&)));

    current_index = -1;
}

TimelineBucket *TimelineWidget::addItem(QString time, QString text)
{
    return addItem(time, QStringList{text});
}

TimelineBucket *TimelineWidget::addItem(QString time, QStringList texts)
{
    return insertItem(time, texts, -1);
}

TimelineBucket *TimelineWidget::insertItem(QString time, QStringList texts, int index)
{
    TimelineBucket* bucket = createItemWidget(time, texts);
    if (index < 0 || index >= count()) // 添加到末尾
    {
        buckets.append(bucket);
        adjustBucketsPositions(count()-1);
    }
    else // 插入到中间
    {
        buckets.insert(index, bucket);
        adjustBucketsPositions(index);
    }

    bucket->setVerticalIndex(count()-1); // 已经添加了，下标索引要-1

    // 设置item的尺寸
    connect(bucket, &TimelineBucket::signalSizeHintChanged, this, [=](QSize size){
        adjustBucketsPositions(buckets.indexOf(bucket)+1);
    });

    // 连接事件信号
    connect(bucket, &TimelineBucket::signalBucketWidgetClicked, this, [=]{ slotBucketWidgetClicked(bucket); });
    connect(bucket, SIGNAL(signalTimeWidgetClicked(TimelineTimeLabel*)), this, SLOT(slotTimeWidgetClicked(TimelineTimeLabel*)));
    connect(bucket, SIGNAL(signalTextWidgetClicked(TimelineTextLabel*)), this, SLOT(slotTextWidgetClicked(TimelineTextLabel*)));
    connect(bucket, SIGNAL(signalTimeWidgetDoubleClicked(TimelineTimeLabel*)), this, SLOT(slotTimeWidgetDoubleClicked(TimelineTimeLabel*)));
    connect(bucket, SIGNAL(signalTextWidgetDoubleClicked(TimelineTextLabel*)), this, SLOT(slotTextWidgetDoubleClicked(TimelineTextLabel*)));

//    connect(bucket, SIGNAL(signalDroppedAndMoved(TimelineBucket*)), this, SLOT(slotDroppedAndMoved(TimelineBucket*)));
    connect(bucket, &TimelineBucket::signalDroppedAndMoved, this, [=](TimelineBucket* from_bucket) {
        slotDroppedAndMoved(from_bucket, bucket);
    });

    updateUI();
    return bucket;
}

void TimelineWidget::removeItem(int index)
{
    if (index < 0 || index >= count())
        return ;

    buckets.takeAt(index)->deleteLater();
}

int TimelineWidget::count()
{
    return buckets.size();
}

void TimelineWidget::selectAll()
{
    foreach (TimelineBucket* bucket, buckets) {
        bucket->setSelected(true);
    }
    selected_buckets = buckets;
}

void TimelineWidget::unselectAll()
{
    foreach (TimelineBucket* bucket, selected_buckets) {
        bucket->setSelected(false);
    }
    selected_buckets.clear();
}

void TimelineWidget::setCurrentItem(int row, bool multi)
{
    if (!multi)
        unselectAll();
    buckets.at(row)->setSelected(true);
    current_index = row;
}

void TimelineWidget::setCurrentItem(TimelineBucket *bucket, bool multi)
{
    if (!multi)
        unselectAll();
    bucket->setSelected(true);
    current_index = buckets.indexOf(bucket);
}

/**
 * 调整某一个位置及后面的所有top
 */
void TimelineWidget::adjustBucketsPositions(int start)
{
    int end = count();
    int top = (start-1) >= 0 ? buckets.at(start-1)->geometry().bottom() : 0;
    for (int i = start; i < end; i++)
    {
        TimelineBucket* bucket = buckets.at(i);
        bucket->move(bucket->pos().x(), top);
        top += bucket->height();
    }
}

/**
 * 调整某一范围内 buckets 的位置
 * 并且包含位置移动动画
 */
void TimelineWidget::adjustBucketsPositionsWithAnimation(int start, int end)
{
    if (end == -1)
        end = count();
    else
        end++;
    int top = (start-1) >= 0 ? buckets.at(start-1)->geometry().bottom() : 0;
    for (int i = start; i < end; i++)
    {
        TimelineBucket* bucket = buckets.at(i);
        QPropertyAnimation* ani = new QPropertyAnimation(bucket, "pos");
        ani->setStartValue(bucket->pos());
        ani->setEndValue(QPoint(bucket->pos().x(), top));
        ani->setDuration(300);
        ani->setEasingCurve(QEasingCurve::OutQuart);
        connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
        ani->start();
        top += bucket->height();
    }
}

TimelineBucket *TimelineWidget::createItemWidget(QString time, QStringList texts)
{
    TimelineBucket* bucket = new TimelineBucket(this);
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

void TimelineWidget::slotBucketWidgetClicked(TimelineBucket *bucket)
{
    if (!bucket->isSelected())
    {
        setCurrentItem(bucket);
    }
    else if (QApplication::keyboardModifiers() == Qt::ControlModifier) // 按下 ctrl
    {
        setCurrentItem(bucket, true);
    }
    else if (QApplication::keyboardModifiers() == Qt::ShiftModifier) // 按下 shift
    {
        int prev = current_index; // 上次按下的
        int curr = buckets.indexOf(bucket);
        if (prev != -1)
        {
            if (prev < curr)
            {
                for (int i = prev; i <= curr; i++)
                {
                    TimelineBucket* bucket = buckets[i];
                    if (!bucket->isSelected())
                    {
                        selected_buckets.append(bucket);
                        bucket->setSelected(true);
                    }
                }
            }
            else if (prev < curr)
            {
                for (int i = prev; i >= curr; i--)
                {
                    TimelineBucket* bucket = buckets[i];
                    if (!bucket->isSelected())
                    {
                        selected_buckets.append(bucket);
                        bucket->setSelected(true);
                    }
                }
            }
        }
        current_index = curr;
    }
}

void TimelineWidget::slotTimeWidgetClicked(TimelineTimeLabel *label)
{

}

void TimelineWidget::slotTextWidgetClicked(TimelineTextLabel *label)
{

}

void TimelineWidget::slotTimeWidgetDoubleClicked(TimelineTimeLabel *label)
{
    QString text = label->text();
    bool ok;
    text = QInputDialog::getText(this, "修改时间", "请输入新的时间", QLineEdit::Normal, text, &ok);
    if (!ok)
        return ;
    label->setText(text);
}

void TimelineWidget::slotTextWidgetDoubleClicked(TimelineTextLabel *label)
{
    QString text = label->text();
    bool ok;
    text = QInputDialog::getText(this, "修改内容", "请输入新的内容", QLineEdit::Normal, text, &ok);
    if (!ok)
        return ;
    label->setText(text);
}

void TimelineWidget::slotMenuShowed(const QPoint &pos)
{
    QMenu* menu = new QMenu("菜单", this);
    QAction* insert_above_action = new QAction("上方插入行", this);
    QAction* insert_under_action = new QAction("下方插入行", this);
    QAction* delete_line_action = new QAction("删除行", this);
    QAction* copy_text_action = new QAction("复制文字", this);
    menu->addAction(insert_above_action);
    menu->addAction(insert_under_action);
    menu->addAction(delete_line_action);
    menu->addAction(copy_text_action);

    if (current_index == -1)
    {
        insert_above_action->setEnabled(false);
        insert_under_action->setEnabled(false);
        delete_line_action->setEnabled(false);
        copy_text_action->setEnabled(false);
    }

    // 设置事件
    connect(insert_above_action, SIGNAL(triggered()), this, SLOT(actionInsertAbove()));
    connect(insert_under_action, SIGNAL(triggered()), this, SLOT(actionInsertUnder()));
    connect(delete_line_action, SIGNAL(triggered()), this, SLOT(actionDeleteLine()));
    connect(copy_text_action, SIGNAL(triggered()), this, SLOT(actionCopyText()));

    menu->exec(QCursor::pos());
}

void TimelineWidget::slotDroppedAndMoved(TimelineBucket *from, TimelineBucket *to)
{
    int from_index = buckets.indexOf(from);
    int to_index = buckets.indexOf(to);
    if (from_index == to_index) // 很可能发生的自己和自己交换
        return ;
    if (from_index < 0 || to_index < 0)
        return ;

    // 交换 bucket
    TimelineBucket* bucket = buckets.at(from_index);
    buckets.removeAt(from_index);
    if (from_index < to_index) // 下移
    {
        buckets.insert(to_index, bucket);
    }
    else // 上移
    {
        buckets.insert(to_index, bucket);
    }
//    adjustBucketsPositions(qMin(from_index, to_index));
    adjustBucketsPositionsWithAnimation(qMin(from_index, to_index), qMax(from_index, to_index));
}

void TimelineWidget::actionInsertAbove()
{
    if (current_index == -1)
        return ;
    setCurrentItem(insertItem("时间节点", QStringList{""}, current_index));
}

void TimelineWidget::actionInsertUnder()
{
    if (current_index == -1)
        return ;
    setCurrentItem(insertItem("时间节点", QStringList{""}, current_index+1));
}

void TimelineWidget::actionDeleteLine()
{
    if (current_index == -1)
        return ;
    removeItem(current_index);
}

void TimelineWidget::actionCopyText()
{

}
