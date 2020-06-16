#include "timelineundostack.h"

TimelineUndoStack::TimelineUndoStack(TimelineWidget *widget) : QUndoStack(widget), widget(widget)
{
    timeline_undos = this;
}

void TimelineUndoStack::addCommand(TimelineBucket *bucket, int index)
{
    push(new TimelineBucketAddCommand(bucket, index));
}

void TimelineUndoStack::addCommand(TimelineBucket *bucket, TimelineTextLabel *label, int index)
{
    push(new TimelineBucketTextAddCommand(bucket, label, index));
}

void TimelineUndoStack::deleteCommand(TimelineBucket *bucket, int index)
{
    push(new TimelineBucketDeleteCommand(bucket, index));
}

void TimelineUndoStack::deleteCommand(TimelineBucket *bucket, TimelineTextLabel *label, int index)
{
    push(new TimelineBucketTextDeleteCommand(bucket, label, index));
}

void TimelineUndoStack::moveCommand(TimelineBucket *bucket, int old_index, int new_index)
{
    push(new TimelineBucketMoveCommand(bucket, old_index, new_index));
}

void TimelineUndoStack::moveCommand(TimelineBucket *bucket, TimelineTextLabel *label, int old_index, int new_index)
{
    push(new TimelineBucketTextMoveCommand(bucket, label, old_index, new_index));
}

void TimelineUndoStack::moveCommand(TimelineBucket *old_bucket, TimelineBucket *new_bucket, TimelineTextLabel *label, int old_index, int new_index)
{
    push(new TimelineBucketTextBucketMoveCommand(old_bucket, new_bucket, label, old_index, new_index));
}

void TimelineUndoStack::modifyCommand(TimelineBucket *bucket, TimelineTimeLabel *label, QString old_text, QString new_text)
{
    push(new TimelineBucketTimeModifyCommand(bucket, label, old_text, new_text));
}

void TimelineUndoStack::modifyCommand(TimelineBucket *bucket, TimelineTextLabel *label, QString old_text, QString new_text)
{
    push(new TimelineBucketTextModifyCommand(bucket, label, old_text, new_text));
}
