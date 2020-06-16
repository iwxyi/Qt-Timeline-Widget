#include "timelineundostack.h"

TimelineUndoStack::TimelineUndoStack(TimelineWidget *widget) : QUndoStack(widget), widget(widget)
{
    timeline_undos = this;
}

void TimelineUndoStack::addCommand(TimelineBucket *bucket, int index)
{

}

void TimelineUndoStack::addCommand(TimelineBucket *bucket, TimelineTextLabel *label, int index)
{

}

void TimelineUndoStack::deleteCommand(TimelineBucket *bucket, int index)
{

}

void TimelineUndoStack::deleteCommand(TimelineBucket *bucket, TimelineTextLabel *label, int index)
{

}

void TimelineUndoStack::moveCommand(TimelineBucket *bucket, int old_index, int new_index)
{

}

void TimelineUndoStack::moveCommand(TimelineBucket *bucket, TimelineTextLabel *label, int old_index, int new_index)
{

}

void TimelineUndoStack::moveCommand(TimelineBucket *old_bucket, TimelineBucket *new_bucket, TimelineTextLabel *label, int old_index, int new_index)
{

}

void TimelineUndoStack::modifyCommand(TimelineBucket *bucket, TimelineTimeLabel *label, QString old_text, QString new_text)
{

}

void TimelineUndoStack::modifyCommand(TimelineBucket *bucket, TimelineTextLabel *label, QString old_text, QString new_text)
{

}
