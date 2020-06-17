#ifndef UNDOCOMMANDS_H
#define UNDOCOMMANDS_H

#include <QUndoCommand>
#include "timelinewidget.h"

class TimelineBucketAddCommand : public QUndoCommand
{
public:
    TimelineBucketAddCommand(TimelineBucket* bucket, int index, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    TimelineBucket* bucket;
    int index;
};


class TimelineBucketTextAddCommand : public QUndoCommand
{
public:
    TimelineBucketTextAddCommand(TimelineBucket* bucket, TimelineTextLabel* label, int index, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    TimelineBucket* bucket;
    TimelineTextLabel* label;
    int index;
};


class TimelineBucketDeleteCommand : public QUndoCommand
{
public:
    TimelineBucketDeleteCommand(TimelineBucket* bucket, int index, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    TimelineBucket* bucket;
    int index;
};


class TimelineBucketTextDeleteCommand : public QUndoCommand
{
public:
    TimelineBucketTextDeleteCommand(TimelineBucket* bucket, TimelineTextLabel* label, int index, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    TimelineBucket* bucket;
    TimelineTextLabel* label;
    int index;
};


class TimelineBucketMoveCommand : public QUndoCommand
{
public:
    TimelineBucketMoveCommand(TimelineBucket* bucket, int old_index, int new_index, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    TimelineBucket* bucket;
    int old_index;
    int new_index;
};


class TimelineBucketTextMoveCommand : public QUndoCommand
{
public:
    TimelineBucketTextMoveCommand(TimelineBucket* bucket, TimelineTextLabel* label, int old_index, int new_index, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    TimelineBucket* bucket;
    TimelineTextLabel* label;
    int old_index;
    int new_index;
};


class TimelineBucketTextBucketMoveCommand : public QUndoCommand
{
public:
    TimelineBucketTextBucketMoveCommand(TimelineBucket* old_bucket, TimelineBucket* new_bucket, int old_index, int new_index, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    TimelineBucket* old_bucket;
    TimelineBucket* new_bucket;
    int old_index;
    int new_index;
};


class TimelineBucketTimeModifyCommand : public QUndoCommand
{
public:
    TimelineBucketTimeModifyCommand(TimelineBucket* bucket, TimelineTimeLabel* label, QString old_text, QString new_text, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    TimelineBucket* bucket;
    TimelineTimeLabel* label;
    QString old_text;
    QString new_text;
};


class TimelineBucketTextModifyCommand : public QUndoCommand
{
public:
    TimelineBucketTextModifyCommand(TimelineBucket* bucket, TimelineTextLabel* label, QString old_text, QString new_text, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    TimelineBucket* bucket;
    TimelineTextLabel* label;
    QString old_text;
    QString new_text;
};

#endif // UNDOCOMMANDS_H
