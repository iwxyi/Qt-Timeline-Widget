#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QObject>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDebug>
#include <QInputDialog>
#include <QMenu>
#include <QAction>
#include "timebucket.h"

class TimelineWidget : public QListWidget
{
    Q_OBJECT
public:
    TimelineWidget(QWidget *parent = nullptr);

    void addItem(QString time, QString text);
    void addItem(QString time, QStringList texts);

private:
    TimeBucket *createItemWidget(QString time, QStringList texts);

signals:

public slots:
    void updateUI();
    void slotTimeWidgetClicked(TimelineTimeLabel* label);
    void slotTextWidgetClicked(TimelineTextLabel* label);
    void slotMenuShowed(const QPoint& pos);

    void actionInsertAbove();
    void actionInsertUnder();
    void actionDeleteLine();
    void actionCopyText();

private:
    QList<TimeBucket*> buckets;
};

#endif // TIMELINEWIDGET_H
