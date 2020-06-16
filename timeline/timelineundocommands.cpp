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

}

void TimelineBucketMoveCommand::undo()
{

}

void TimelineBucketMoveCommand::redo()
{

}

TimelineBucketTextMoveCommand::TimelineBucketTextMoveCommand(TimelineBucket *bucket, TimelineTextLabel *label, int old_index, int new_index, QUndoCommand *parent)
	: QUndoCommand(parent), bucket(bucket), label(label), old_index(old_index), new_index(new_index)
{

}

void TimelineBucketTextMoveCommand::undo()
{

}

void TimelineBucketTextMoveCommand::redo()
{

}

TimelineBucketTextBucketMoveCommand::TimelineBucketTextBucketMoveCommand(TimelineBucket *old_bucket, TimelineBucket *new_bucket, TimelineTextLabel *label, int old_index, int new_index, QUndoCommand *parent)
	: QUndoCommand(parent), old_bucket(old_bucket), new_bucket(new_bucket), label(label), old_index(old_index), new_index(new_index)
{

}

void TimelineBucketTextBucketMoveCommand::undo()
{

}

void TimelineBucketTextBucketMoveCommand::redo()
{

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
