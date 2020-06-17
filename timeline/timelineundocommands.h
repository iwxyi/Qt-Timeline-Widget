#ifndef UNDOCOMMANDS_H
#define UNDOCOMMANDS_H

#include <QUndoCommand>
#include "timelinewidget.h"

class TimelineBucketAddCommand : public QUndoCommand
{
public:
    TimelineBucketAddCommand(TimelineWidget* widget, int index, QUndoCommand* parent = nullptr);
    TimelineBucketAddCommand(TimelineWidget* widget, int index, QString time, QUndoCommand* parent = nullptr);
    TimelineBucketAddCommand(TimelineWidget* widget, int index, QString time, QString text, QUndoCommand* parent = nullptr);
    TimelineBucketAddCommand(TimelineWidget* widget, int index, QString time, QStringList texts, QUndoCommand* parent = nullptr);
    TimelineBucketAddCommand(TimelineWidget* widget, QList<int> indexes, QUndoCommand* parent = nullptr);
    TimelineBucketAddCommand(TimelineWidget* widget, QList<int> indexes, QStringList times, QUndoCommand* parent = nullptr);
    TimelineBucketAddCommand(TimelineWidget* widget, QList<int> indexes, QStringList times, QList<QStringList> textss, QUndoCommand* parent = nullptr);
    TimelineBucketAddCommand(TimelineWidget* widget, QList<int> indexes, QList<QPair<QString, QStringList>> line_texts, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    static QStringList numStringList(int number);
    static QList<QPair<QString, QStringList>> string2lineTexts(QStringList list);
    static QList<QPair<QString, QStringList>> string2lineTexts(QStringList list, QList<QStringList> textss);

private:
    TimelineWidget* widget;
    QList<int> indexes; // 这是插入位置（改动之前的索引，上面的插入会引起下面索引的变动）
    QList<QPair<QString, QStringList>> line_texts;
};


class TimelineBucketTextAddCommand : public QUndoCommand
{
public:
    TimelineBucketTextAddCommand(TimelineWidget* widget, int bucket_index, int index, QUndoCommand *parent = nullptr);
    TimelineBucketTextAddCommand(TimelineWidget* widget, int bucket_index, QList<int> indexes, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    TimelineWidget* widget;
    int bucket_index;
    QList<int> indexes;
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
