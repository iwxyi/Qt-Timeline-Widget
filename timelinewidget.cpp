#include "timelinewidget.h"

TimelineWidget::TimelineWidget(QWidget *parent) : QListWidget(parent)
{
    setAcceptDrops(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested (const QPoint&)),this,SLOT(slotMenuShowed(const QPoint&)));
}

QListWidgetItem *TimelineWidget::addItem(QString time, QString text)
{
    return addItem(time, QStringList{text});
}

QListWidgetItem *TimelineWidget::addItem(QString time, QStringList texts)
{
    return insertItem(time, texts, -1);
}

QListWidgetItem *TimelineWidget::insertItem(QString time, QStringList texts, int index)
{
    QListWidgetItem* item = new QListWidgetItem();
    TimeBucket* bucket = createItemWidget(time, texts);
    if (index < 0 || index >= count()) // 添加到末尾
    {
        buckets.append(bucket);
        QListWidget::addItem(item);
    }
    else // 插入到中间
    {
        buckets.insert(index, bucket);
        QListWidget::insertItem(index, item);
    }

    bucket->setVerticalIndex(count()-1); // 已经添加了，下标索引要-1
    setItemWidget(item, bucket);
    item->setSizeHint(bucket->getSuitableSize());

    // 设置item的尺寸
    connect(bucket, &TimeBucket::signalSizeHintChanged, this, [=](QSize size){
        item->setSizeHint(size);
    });

    // 连接事件信号
    connect(bucket, SIGNAL(signalTimeWidgetClicked(TimelineTimeLabel*)), this, SLOT(slotTimeWidgetClicked(TimelineTimeLabel*)));
    connect(bucket, SIGNAL(signalTextWidgetClicked(TimelineTextLabel*)), this, SLOT(slotTextWidgetClicked(TimelineTextLabel*)));

    updateUI();
    return item;
}

void TimelineWidget::removeItem(int index)
{
    if (index < 0 || index >= count())
        return ;

    QListWidget::takeItem(index);
    buckets.takeAt(index)->deleteLater();
}

void TimelineWidget::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "TimelineWidget::dragEnterEvent";
    event->accept();
    QListWidget::dragEnterEvent(event);
}

void TimelineWidget::dropEvent(QDropEvent *event)
{
    qDebug() << "TimelineWidget::dropEvent";

    QListWidget::dropEvent(event);
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

    if (!currentIndex().isValid())
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

void TimelineWidget::slotStartDrag(TimeBucket *bucket)
{

}

void TimelineWidget::actionInsertAbove()
{
    if (!currentIndex().isValid())
        return ;
    int index = currentRow();
    setCurrentItem(insertItem("时间节点", QStringList{""}, index));
}

void TimelineWidget::actionInsertUnder()
{
    if (!currentIndex().isValid())
        return ;
    int index = currentRow();
    setCurrentItem(insertItem("时间节点", QStringList{""}, index+1));
}

void TimelineWidget::actionDeleteLine()
{
    if (!currentIndex().isValid())
        return ;
    int index = currentRow();
    removeItem(index);
}

void TimelineWidget::actionCopyText()
{

}
