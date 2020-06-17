#include "timelineundocommands.h"

TimelineBucketAddCommand::TimelineBucketAddCommand(TimelineBucket *bucket, int index, QUndoCommand *parent)
    : QUndoCommand(parent), bucket(bucket), index(index)
{

}

void TimelineBucketAddCommand::undo()
{

}

void TimelineBucketAddCommand::redo()
{

}

TimelineBucketTextAddCommand::TimelineBucketTextAddCommand(TimelineBucket *bucket, TimelineTextLabel *label, int index, QUndoCommand *parent)
    : QUndoCommand(parent), bucket(bucket), label(label), index(index)
{

}

void TimelineBucketTextAddCommand::undo()
{

}

void TimelineBucketTextAddCommand::redo()
{

}

TimelineBucketDeleteCommand::TimelineBucketDeleteCommand(TimelineBucket *bucket, int index, QUndoCommand *parent)
	: QUndoCommand(parent), bucket(bucket), index(index)
{

}

void TimelineBucketDeleteCommand::undo()
{

}

void TimelineBucketDeleteCommand::redo()
{

}

TimelineBucketTextDeleteCommand::TimelineBucketTextDeleteCommand(TimelineBucket *bucket, TimelineTextLabel *label, int index, QUndoCommand *parent)
	: QUndoCommand(parent), bucket(bucket), label(label), index(index)
{

}

void TimelineBucketTextDeleteCommand::undo()
{

}

void TimelineBucketTextDeleteCommand::redo()
{

}

TimelineBucketMoveCommand::TimelineBucketMoveCommand(TimelineWidget *widget, int old_index, int new_index, QUndoCommand *parent)
    : QUndoCommand(parent), widget(widget), old_index(old_index), new_index(new_index)
{

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

}

void TimelineBucketTextMoveCommand::undo()
{
    widget->at(bucket_index)->moveTextLabel(new_index, old_index);
}

void TimelineBucketTextMoveCommand::redo()
{
    widget->at(bucket_index)->moveTextLabel(old_index, new_index);
}

TimelineBucketTextBucketMoveCommand::TimelineBucketTextBucketMoveCommand(TimelineWidget *widget, int old_bucket_index, int new_bucket_index, int old_index, int new_index, QUndoCommand *parent)
    : QUndoCommand(parent), widget(widget), old_bucket_index(old_bucket_index), new_bucket_index(new_bucket_index), old_index(old_index), new_index(new_index)
{

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

TimelineBucketTimeModifyCommand::TimelineBucketTimeModifyCommand(TimelineBucket *bucket, TimelineTimeLabel *label, QString old_text, QString new_text, QUndoCommand *parent)
	: QUndoCommand(parent), bucket(bucket), label(label), old_text(old_text), new_text(new_text)
{

}

void TimelineBucketTimeModifyCommand::undo()
{

}

void TimelineBucketTimeModifyCommand::redo()
{

}

TimelineBucketTextModifyCommand::TimelineBucketTextModifyCommand(TimelineBucket *bucket, TimelineTextLabel *label, QString old_text, QString new_text, QUndoCommand *parent)
	: QUndoCommand(parent), bucket(bucket), old_text(old_text), new_text(new_text)
{

}

void TimelineBucketTextModifyCommand::undo()
{

}

void TimelineBucketTextModifyCommand::redo()
{

}
