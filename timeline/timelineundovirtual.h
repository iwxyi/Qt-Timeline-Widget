#ifndef TIMELINEUNDOVIRTUAL_H
#define TIMELINEUNDOVIRTUAL_H

#include <QString>
#include <QList>

class TimelineWidget;
class TimelineBucket;
class TimelineTimeLabel;
class TimelineTextLabel;

class TimelineUndoVirtual
{
public:
    TimelineUndoVirtual();
    virtual ~TimelineUndoVirtual();

    virtual void addCommand(int index) = 0;
    virtual void addCommand(QList<int> indexes) = 0;
    virtual void addCommand(TimelineBucket* bucket, int index) = 0;
    virtual void addCommand(TimelineBucket* bucket, QList<int> indexes) = 0;
    virtual void deleteCommand(int index) = 0;
    virtual void deleteCommand(QList<int> indexes) = 0;
    virtual void moveCommand(int old_index, int new_index) = 0;
    virtual void moveCommand(TimelineBucket* bucket, int old_index, int new_index) = 0;
    virtual void moveCommand(TimelineBucket* old_bucket, TimelineBucket* new_bucket, int old_index, int new_index) = 0;
    virtual void modifyCommand(TimelineBucket* bucket, QString old_text, QString new_text) = 0;
    virtual void modifyCommand(TimelineBucket* bucket, TimelineTextLabel* label, QString old_text, QString new_text) = 0;

    virtual void undoCommand() = 0;
    virtual void redoCommand() = 0;

};

#endif // TIMELINEUNDOVIRTUAL_H
