#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QObject>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDebug>
#include <QInputDialog>
#include <QMenu>
#include <QAction>
#include <QMimeData>
#include <QDrag>
#include "timelinebucket.h"

class TimelineWidget : public QListWidget
{
    Q_OBJECT
public:
    TimelineWidget(QWidget *parent = nullptr);

    QListWidgetItem* addItem(QString time, QString text);
    QListWidgetItem* addItem(QString time, QStringList texts);
    QListWidgetItem* insertItem(QString time, QStringList texts, int index = -1);
    void removeItem(int index);

protected:

private:
    TimelineBucket *createItemWidget(QString time, QStringList texts);

signals:

public slots:
    void updateUI();
    void slotTimeWidgetClicked(TimelineTimeLabel* label);
    void slotTextWidgetClicked(TimelineTextLabel* label);
    void slotTimeWidgetDoubleClicked(TimelineTimeLabel* label);
    void slotTextWidgetDoubleClicked(TimelineTextLabel* label);
    void slotMenuShowed(const QPoint& pos);

    void actionInsertAbove();
    void actionInsertUnder();
    void actionDeleteLine();
    void actionCopyText();

private:
    QList<TimelineBucket*> buckets;
};

#endif // TIMELINEWIDGET_H
