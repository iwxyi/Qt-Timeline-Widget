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
    TimelineBucketMoveCommand(TimelineWidget* widget, int old_index, int new_index, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    TimelineWidget* widget;
    int old_index;
    int new_index;
};


class TimelineBucketTextMoveCommand : public QUndoCommand
{
public:
    TimelineBucketTextMoveCommand(TimelineWidget* widget, int bucket_index, int old_index, int new_index, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    TimelineWidget* widget;
    int bucket_index;
    int old_index;
    int new_index;
};


class TimelineBucketTextBucketMoveCommand : public QUndoCommand
{
public:
    TimelineBucketTextBucketMoveCommand(TimelineWidget* widget, int old_bucket_index, int new_bucket_index, int old_index, int new_index, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    TimelineWidget* widget;
    int old_bucket_index;
    int new_bucket_index;
    int old_index;
    int new_index;
};


class TimelineBucketTimeModifyCommand : public QUndoCommand
{
public:
    TimelineBucketTimeModifyCommand(TimelineWidget* widget, int bucket_index, QString old_text, QString new_text, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    TimelineWidget* widget;
    int bucket_index;
    QString old_text;
    QString new_text;
};


class TimelineBucketTextModifyCommand : public QUndoCommand
{
public:
    TimelineBucketTextModifyCommand(TimelineWidget* widget, int bucket_index, int label_index, QString old_text, QString new_text, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    TimelineWidget* widget;
    int bucket_index;
    int label_index;
    QString old_text;
    QString new_text;
};

#endif // UNDOCOMMANDS_H
