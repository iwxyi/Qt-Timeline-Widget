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

TimelineBucketMoveCommand::TimelineBucketMoveCommand(TimelineBucket *bucket, int old_index, int new_index, QUndoCommand *parent)
	: QUndoCommand(parent), bucket(bucket), old_index(old_index), new_index(new_index)
{
    qDebug() << "构造";
}

void TimelineBucketMoveCommand::undo()
{
    qDebug() << "撤销";
    bucket->moveTextLabel(new_index, old_index);
}

void TimelineBucketMoveCommand::redo()
{
    qDebug() << "重做";
    bucket->moveTextLabel(old_index, new_index);
}

TimelineBucketTextMoveCommand::TimelineBucketTextMoveCommand(TimelineBucket *bucket, TimelineTextLabel *label, int old_index, int new_index, QUndoCommand *parent)
	: QUndoCommand(parent), bucket(bucket), label(label), old_index(old_index), new_index(new_index)
{

}

void TimelineBucketTextMoveCommand::undo()
{
    bucket->moveTextLabel(new_index, old_index);
}

void TimelineBucketTextMoveCommand::redo()
{
    bucket->moveTextLabel(old_index, new_index);
}

TimelineBucketTextBucketMoveCommand::TimelineBucketTextBucketMoveCommand(TimelineBucket *old_bucket, TimelineBucket *new_bucket, int old_index, int new_index, QUndoCommand *parent)
    : QUndoCommand(parent), old_bucket(old_bucket), new_bucket(new_bucket), old_index(old_index), new_index(new_index)
{

}

void TimelineBucketTextBucketMoveCommand::undo()
{
    auto label = new_bucket->at(new_index);
    old_bucket->createTextLabel(label, old_index, QPoint(label->x(), label->getGlobalPos().y() - old_bucket->pos().y()));
    label->draggedToOut();

    old_bucket->adjustBucketSize();
    old_bucket->adjustWidgetsPositionsWithAnimation();
}

void TimelineBucketTextBucketMoveCommand::redo()
{
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
