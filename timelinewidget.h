#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QObject>
#include <QDebug>
#include <QInputDialog>
#include <QMenu>
#include <QAction>
#include <QMimeData>
#include <QDrag>
#include "timelinebucket.h"

class TimelineWidget : public QWidget
{
    Q_OBJECT
public:
    TimelineWidget(QWidget *parent = nullptr);

    TimelineBucket* addItem(QString time, QString text);
    TimelineBucket* addItem(QString time, QStringList texts);
    TimelineBucket* insertItem(QString time, QStringList texts, int index = -1);
    void removeItem(int index);
    int count();

    void selectAll();
    void unselectAll();
    void setCurrentItem(int row, bool multi = false);
    void setCurrentItem(TimelineBucket* bucket, bool multi = false);

    void adjustBucketsPositions(int start);

protected:

private:
    TimelineBucket *createItemWidget(QString time, QStringList texts);

signals:

public slots:
    void updateUI();
    void slotBucketWidgetClicked(TimelineBucket* bucket);
    void slotTimeWidgetClicked(TimelineTimeLabel* label);
    void slotTextWidgetClicked(TimelineTextLabel* label);
    void slotTimeWidgetDoubleClicked(TimelineTimeLabel* label);
    void slotTextWidgetDoubleClicked(TimelineTextLabel* label);
    void slotMenuShowed(const QPoint& pos);
    void slotDroppedAndMoved(TimelineBucket* from, TimelineBucket* to);

    void actionInsertAbove();
    void actionInsertUnder();
    void actionDeleteLine();
    void actionCopyText();

private:
    QList<TimelineBucket*> buckets;
    QList<TimelineBucket*> selected_buckets;
    int current_index;
};

#endif // TIMELINEWIDGET_H
