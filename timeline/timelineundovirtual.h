#ifndef TIMELINEUNDOVIRTUAL_H
#define TIMELINEUNDOVIRTUAL_H

class QString;

class TimelineWidget;
class TimelineBucket;
class TimelineTimeLabel;
class TimelineTextLabel;

class TimelineUndoVirtual
{
public:
    TimelineUndoVirtual();
    virtual ~TimelineUndoVirtual();

    virtual void addCommand(TimelineBucket* bucket, int index) = 0;
    virtual void addCommand(TimelineBucket* bucket, TimelineTextLabel* label, int index) = 0;
    virtual void moveCommand(TimelineBucket* bucket, int old_index, int new_index) = 0;
    virtual void moveCommand(TimelineBucket* bucket, TimelineTextLabel* label, int old_index, int new_index) = 0;
    virtual void moveCommand(TimelineBucket* old_bucket, TimelineBucket* new_bucket, TimelineTextLabel* label, int old_index, int new_index) = 0;
    virtual void deleteCommand(TimelineBucket* bucket, int index) = 0;
    virtual void deleteCommand(TimelineBucket* bucket, TimelineTextLabel* label, int index) = 0;
    virtual void modifyCommand(TimelineBucket* bucket, TimelineTimeLabel* label, QString old_text, QString new_text) = 0;
    virtual void modifyCommand(TimelineBucket* bucket, TimelineTextLabel* label, QString old_text, QString new_text) = 0;

};

#endif // TIMELINEUNDOVIRTUAL_H
