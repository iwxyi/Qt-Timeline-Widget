#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QObject>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDebug>
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

private:
    QList<TimeBucket*> buckets;
};

#endif // TIMELINEWIDGET_H
