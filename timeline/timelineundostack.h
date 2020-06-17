#ifndef TIMELINEUNDOSTACK_H
#define TIMELINEUNDOSTACK_H

#include <QObject>
#include <QUndoStack>
#include <QAction>
#include "timelinewidget.h"
#include "timelineundocommands.h"

class TimelineUndoStack : public QUndoStack, public TimelineUndoVirtual
{
public:
    TimelineUndoStack(TimelineWidget* widget);

    virtual void addCommand(int index) override;
    virtual void addCommand(QList<int> indexes) override;
    virtual void deleteCommand(TimelineBucket* bucket, int index) override;
    virtual void deleteCommand(TimelineBucket* bucket, TimelineTextLabel* label, int index) override;
    virtual void moveCommand(int old_index, int new_index) override;
    virtual void moveCommand(TimelineBucket* bucket, int old_index, int new_index) override;
    virtual void moveCommand(TimelineBucket* old_bucket, TimelineBucket* new_bucket, int old_index, int new_index) override;
    virtual void modifyCommand(TimelineBucket* bucket, QString old_text, QString new_text) override;
    virtual void modifyCommand(TimelineBucket* bucket, TimelineTextLabel* label, QString old_text, QString new_text) override;

    virtual void undoCommand() override;
    virtual void redoCommand() override;

private:
    TimelineWidget* widget;
};

#endif // TIMELINEUNDOSTACK_H
