#include "timelineundocommands.h"

TimelineBucketAddCommand::TimelineBucketAddCommand(TimelineWidget *widget, int index, QUndoCommand *parent)
    : TimelineBucketAddCommand(widget, index, "", parent)
{

}

TimelineBucketAddCommand::TimelineBucketAddCommand(TimelineWidget *widget, int index, QString time, QUndoCommand *parent)
    : TimelineBucketAddCommand(widget, index, time, "", parent)
{

}

TimelineBucketAddCommand::TimelineBucketAddCommand(TimelineWidget *widget, int index, QString time, QString text, QUndoCommand *parent)
    : TimelineBucketAddCommand(widget, index, time, QStringList{text}, parent)
{

}

TimelineBucketAddCommand::TimelineBucketAddCommand(TimelineWidget *widget, int index, QString time, QStringList texts, QUndoCommand *parent)
    : TimelineBucketAddCommand(widget, QList<int>{index}, QList<QPair<QString, QStringList>>{{time, texts}}, parent)
{

}

TimelineBucketAddCommand::TimelineBucketAddCommand(TimelineWidget *widget, QList<int> indexes, QUndoCommand *parent)
    : TimelineBucketAddCommand(widget, indexes, numStringList(indexes.size()), parent)
{

}

TimelineBucketAddCommand::TimelineBucketAddCommand(TimelineWidget *widget, QList<int> indexes, QStringList times, QUndoCommand *parent)
    : TimelineBucketAddCommand(widget, indexes, string2lineTexts(times), parent)
{

}

TimelineBucketAddCommand::TimelineBucketAddCommand(TimelineWidget *widget, QList<int> indexes, QStringList times, QList<QStringList> textss, QUndoCommand *parent)
    : TimelineBucketAddCommand(widget, indexes, string2lineTexts(times, textss), parent)
{

}

TimelineBucketAddCommand::TimelineBucketAddCommand(TimelineWidget *widget, QList<int> indexes, QList<QPair<QString, QStringList> > line_texts, QUndoCommand *parent)
    : QUndoCommand(parent), widget(widget), indexes(indexes), line_texts(line_texts)
{
    setText("添加行");
}

void TimelineBucketAddCommand::undo()
{
    for (int i = 0; i < indexes.size(); i++)
    {
        widget->removeItem(indexes.at(i));
    }
    widget->resetWidth();
    widget->adjustBucketsPositionsWithAnimation();
}

void TimelineBucketAddCommand::redo()
{
    int cumu = 0; // 上面插入影响下面索引的累加变化量
    for (int i = 0; i < indexes.size(); i++)
    {
        if (indexes.at(i) == -1) // 默认末尾，设置为最后一项
            indexes[i] = widget->count();
        widget->insertItem(line_texts.at(i).first, line_texts.at(i).second, indexes.at(i)+cumu);
        cumu++;
    }
    widget->resetWidth();
    widget->adjustBucketsPositionsWithAnimation();
}

TimelineBucketTextAddCommand::TimelineBucketTextAddCommand(TimelineWidget *widget, int bucket_index, int index, QUndoCommand *parent)
    : TimelineBucketTextAddCommand(widget, bucket_index, QList<int>{index}, parent)
{

}

TimelineBucketTextAddCommand::TimelineBucketTextAddCommand(TimelineWidget *widget, int bucket_index, QList<int> indexes, QUndoCommand *parent)
    : QUndoCommand(parent), widget(widget), bucket_index(bucket_index), indexes(indexes)
{
    setText("添加文字");
}

void TimelineBucketTextAddCommand::undo()
{
    auto bucket = widget->at(bucket_index);
    for (int i = 0; i < indexes.size(); i++)
    {
        bucket->removeAt(indexes.at(i));
    }
    bucket->adjustWidgetsPositionsWithAnimation(indexes.first());
    bucket->adjustBucketSize();
}

void TimelineBucketTextAddCommand::redo()
{
    auto bucket = widget->at(bucket_index);
    for (int i = indexes.size()-1; i >= 0; i--)
    {
        bucket->insertTextWidget("", indexes.at(i));
    }
    bucket->adjustWidgetsPositionsWithAnimation(indexes.first());
    bucket->adjustBucketSize();
}

TimelineBucketTextsAddCommand::TimelineBucketTextsAddCommand(TimelineWidget *widget, int bucket_index, int text_index, QUndoCommand *parent)
    : TimelineBucketTextsAddCommand(widget, QList<int>{bucket_index}, QList<int>{text_index}, parent)
{

}

TimelineBucketTextsAddCommand::TimelineBucketTextsAddCommand(TimelineWidget *widget, QList<int> bucket_indexes, QList<int> text_indexes, QUndoCommand *parent)
    : TimelineBucketTextsAddCommand(widget, bucket_indexes, intRow2Col(text_indexes), parent)
{

}

TimelineBucketTextsAddCommand::TimelineBucketTextsAddCommand(TimelineWidget *widget, QList<int> bucket_indexes, QList<QList<int> > texts_indexes, QUndoCommand *parent)
    : QUndoCommand(parent), widget(widget), bucket_indexes(bucket_indexes), texts_indexes(texts_indexes)
{
    setText("插入文字节点");
}

void TimelineBucketTextsAddCommand::undo()
{
    for (int i = 0; i < bucket_indexes.size(); i++) // 每一个bucket
    {
        auto bucket = widget->at(bucket_indexes.at(i));
        auto indexes = texts_indexes.at(i);
        for (int j = indexes.size()-1; j >= 0; j--) // 每一个 TextLabel
        {
            bucket->removeAt(indexes.at(j));
        }
        bucket->adjustWidgetsPositionsWithAnimation(indexes.first());
    }
}

void TimelineBucketTextsAddCommand::redo()
{
    for (int i = 0; i < bucket_indexes.size(); i++) // 每一个bucket
    {
        auto bucket = widget->at(bucket_indexes.at(i));
        auto indexes = texts_indexes.at(i);
        for (int j = indexes.size()-1; j >= 0; j--) // 每一个 TextLabel
        {
            bucket->insertTextWidget("", indexes.at(j));
        }
        bucket->adjustWidgetsPositionsWithAnimation(indexes.first());
    }
}

QList<QList<int> > TimelineBucketTextsAddCommand::intRow2Col(QList<int> row)
{
    QList<QList<int>> cols;
    foreach (auto i, row)
    {
        cols << QList<int>{i};
    }
    return cols;
}


QStringList TimelineBucketAddCommand::numStringList(int number)
{
    QStringList result;
    while (number-- >= 0)
        result.append("");
    return result;
}

QList<QPair<QString, QStringList> > TimelineBucketAddCommand::string2lineTexts(QStringList list)
{
    QList<QPair<QString, QStringList> > result;
    foreach (auto li, list)
        result.append(QPair<QString, QStringList>{li, QStringList{}});
    return result;
}

QList<QPair<QString, QStringList> > TimelineBucketAddCommand::string2lineTexts(QStringList list, QList<QStringList> textss)
{
    int size = qMin(list.size(), textss.size());
    QList<QPair<QString, QStringList> > result;
    for (int i = 0; i < size; i++)
        result.append(QPair<QString, QStringList>{list.at(i), textss.at(i)});
    return result;
}

TimelineBucketDeleteCommand::TimelineBucketDeleteCommand(TimelineWidget *widget, int index, QUndoCommand *parent)
    : TimelineBucketDeleteCommand(widget, QList<int>{index}, parent)
{

}

TimelineBucketDeleteCommand::TimelineBucketDeleteCommand(TimelineWidget *widget, QList<int> indexes, QUndoCommand *parent)
    : QUndoCommand(parent), widget(widget), indexes(indexes)
{

}

void TimelineBucketDeleteCommand::undo()
{
    for (int i = 0; i < indexes.size(); i++)
    {
        widget->insertItem(line_texts.at(i).first, line_texts.at(i).second, indexes.at(i));
    }
    widget->resetWidth();
    widget->adjustBucketsPositionsWithAnimation(indexes.first());
}

void TimelineBucketDeleteCommand::redo()
{
    line_texts.clear();
    for (int i = indexes.size()-1; i >= 0; i--)
    {
        auto bucket = widget->at(indexes.at(i));
        line_texts.insert(0, QPair<QString, QStringList>{bucket->getTime(), bucket->getTexts()});
        widget->removeItem(indexes.at(i));
    }
    widget->resetWidth();
    widget->adjustBucketsPositionsWithAnimation(indexes.first());
}

TimelineBucketTextDeleteCommand::TimelineBucketTextDeleteCommand(TimelineWidget *widget, int bucket_index, int index, QUndoCommand *parent)
    : TimelineBucketTextDeleteCommand(widget, bucket_index, QList<int>{index}, parent)
{

}

TimelineBucketTextDeleteCommand::TimelineBucketTextDeleteCommand(TimelineWidget *widget, int bucket_index, QList<int> indexes, QUndoCommand *parent)
    : QUndoCommand(parent), widget(widget), bucket_index(bucket_index), indexes(indexes)
{
    setText("删除文字");
}

void TimelineBucketTextDeleteCommand::undo()
{
    auto bucket = widget->at(bucket_index);
    for (int i = 0; i < indexes.size(); i++)
    {
        bucket->insertTextWidget(texts.at(i), indexes.at(i));
    }
    bucket->adjustWidgetsPositionsWithAnimation(indexes.first());
    bucket->adjustBucketSize();
}

void TimelineBucketTextDeleteCommand::redo()
{
    auto bucket = widget->at(bucket_index);
    texts.clear();
    foreach (auto index, indexes)
    {
        texts.append(bucket->getText(index));
    }

    for (int i = indexes.size()-1; i >= 0; i--)
    {
        bucket->removeAt(indexes.at(i));
    }
    bucket->adjustWidgetsPositionsWithAnimation(indexes.first());
    bucket->adjustBucketSize();
}

TimelineBucketMoveCommand::TimelineBucketMoveCommand(TimelineWidget *widget, int old_index, int new_index, QUndoCommand *parent)
    : QUndoCommand(parent), widget(widget), old_index(old_index), new_index(new_index)
{
    setText("移动行");
}

void TimelineBucketMoveCommand::undo()
{
    widget->moveBucket(new_index, old_index);
}

void TimelineBucketMoveCommand::redo()
{
    widget->moveBucket(old_index, new_index);
}

TimelineBucketTextMoveCommand::TimelineBucketTextMoveCommand(TimelineWidget *widget, int bucket_index, int old_index, int new_index, QUndoCommand *parent)
    : QUndoCommand(parent), widget(widget), bucket_index(bucket_index), old_index(old_index), new_index(new_index)
{
    setText("移动文字");
}

void TimelineBucketTextMoveCommand::undo()
{
    widget->at(bucket_index)->actionMoveTextLabel(new_index, old_index);
}

void TimelineBucketTextMoveCommand::redo()
{
    widget->at(bucket_index)->actionMoveTextLabel(old_index, new_index);
}

TimelineBucketTextBucketMoveCommand::TimelineBucketTextBucketMoveCommand(TimelineWidget *widget, int old_bucket_index, int new_bucket_index, int old_index, int new_index, QUndoCommand *parent)
    : QUndoCommand(parent), widget(widget), old_bucket_index(old_bucket_index), new_bucket_index(new_bucket_index), old_index(old_index), new_index(new_index)
{
    setText("移动文字");
}

void TimelineBucketTextBucketMoveCommand::undo()
{
    auto new_bucket = widget->at(new_bucket_index);
    auto old_bucket = widget->at(old_bucket_index);
    auto label = new_bucket->at(new_index);
    old_bucket->createTextLabel(label, old_index, QPoint(label->x(), label->getGlobalPos().y() - old_bucket->pos().y()));
    label->draggedToOut();

    old_bucket->adjustBucketSize();
    old_bucket->adjustWidgetsPositionsWithAnimation();
}

void TimelineBucketTextBucketMoveCommand::redo()
{
    auto new_bucket = widget->at(new_bucket_index);
    auto old_bucket = widget->at(old_bucket_index);
    auto label = old_bucket->at(old_index);
    new_bucket->createTextLabel(label, new_index, QPoint(label->x(), label->getGlobalPos().y() - new_bucket->pos().y()));
    label->draggedToOut(); // 从父类那里删掉

    new_bucket->adjustBucketSize(); // 从其他bucket那里移动过来，需要手动更换位置
    new_bucket->adjustWidgetsPositionsWithAnimation();
}

TimelineBucketTimeModifyCommand::TimelineBucketTimeModifyCommand(TimelineWidget *widget, int bucket_index, QString old_text, QString new_text, QUndoCommand *parent)
    : QUndoCommand(parent), widget(widget), bucket_index(bucket_index), old_text(old_text), new_text(new_text)
{
    setText("修改时间");
}

void TimelineBucketTimeModifyCommand::undo()
{
    auto bucket = widget->at(bucket_index);
    auto label = bucket->timeLabel();
    label->setText(old_text);
    label->adjustSize();
    bucket->adjustWidgetsPositionsWithAnimation();
    widget->adjustBucketsPositionsWithAnimation();
}

void TimelineBucketTimeModifyCommand::redo()
{
    auto bucket = widget->at(bucket_index);
    auto label = bucket->timeLabel();
    label->setText(new_text);
    label->adjustSize();
    bucket->adjustWidgetsPositionsWithAnimation();
    widget->adjustBucketsPositionsWithAnimation();
}

TimelineBucketTextModifyCommand::TimelineBucketTextModifyCommand(TimelineWidget *widget, int bucket_index, int label_index, QString old_text, QString new_text, QUndoCommand *parent)
    : QUndoCommand(parent), widget(widget), bucket_index(bucket_index), label_index(label_index), old_text(old_text), new_text(new_text)
{
    setText("修改文字");
}

void TimelineBucketTextModifyCommand::undo()
{
    auto bucket = widget->at(bucket_index);
    auto label = bucket->at(label_index);
    label->setText(old_text);
    label->adjustSize();
    bucket->adjustWidgetsPositionsWithAnimation();
    widget->adjustBucketsPositionsWithAnimation();
}

void TimelineBucketTextModifyCommand::redo()
{
    auto bucket = widget->at(bucket_index);
    auto label = bucket->at(label_index);
    label->setText(new_text);
    label->adjustSize();
    bucket->adjustWidgetsPositionsWithAnimation();
    widget->adjustBucketsPositionsWithAnimation();
}
