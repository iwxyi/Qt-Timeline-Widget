#ifndef TIMELINEUNDOSTACK_H
#define TIMELINEUNDOSTACK_H

#include <QObject>
#include <QUndoStack>
#include "timelinewidget.h"
#include "timelineundocommands.h"

class TimelineUndoStack : public QUndoStack, public TimelineUndoVirtual
{
public:
    TimelineUndoStack(TimelineWidget* widget);

    virtual void addCommand(TimelineBucket* bucket, int index);
    virtual void addCommand(TimelineBucket* bucket, TimelineTextLabel* label, int index);
    virtual void deleteCommand(TimelineBucket* bucket, int index);
    virtual void deleteCommand(TimelineBucket* bucket, TimelineTextLabel* label, int index);
    virtual void moveCommand(TimelineBucket* bucket, int old_index, int new_index);
    virtual void moveCommand(TimelineBucket* bucket, TimelineTextLabel* label, int old_index, int new_index);
    virtual void moveCommand(TimelineBucket* old_bucket, TimelineBucket* new_bucket, TimelineTextLabel* label, int old_index, int new_index);
    virtual void modifyCommand(TimelineBucket* bucket, TimelineTimeLabel* label, QString old_text, QString new_text);
    virtual void modifyCommand(TimelineBucket* bucket, TimelineTextLabel* label, QString old_text, QString new_text);

private:
    TimelineWidget* widget;
};

#endif // TIMELINEUNDOSTACK_H
