#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QObject>
#include <QListWidget>
#include <QListWidgetItem>
#include "timebucket.h"

class TimelineWidget : public QListWidget
{
    Q_OBJECT
public:
    TimelineWidget(QWidget *parent = nullptr);

    void addItem(QString time, QString text);

private:
    TimeBucket *createItemWidget(QString time, QString text);
    TimeBucket *createItemWidget(QString time, QStringList texts);

signals:

public slots:

private:

};

#endif // TIMELINEWIDGET_H
