#ifndef TIMEBUCKET_H
#define TIMEBUCKET_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include "timelinetimelabel.h"
#include "timelinetextlabel.h"

class TimeBucket : public QWidget
{
    Q_OBJECT
public:
    TimeBucket(QWidget *parent = nullptr);
    void initView();

    void setTime(QString time);
    void setText(int index, QString text);
    void setText(QString text);
    void setText(QStringList texts);

    void addTextWidget(QString text = "");

    void clearText();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void addOneLabel(QString text);

signals:

public slots:

private:
    QHBoxLayout* hlayout;
    TimelineTimeLabel* time_widget; // 时间点的线
    QList<TimelineTextLabel*> text_widgets; // 这个时间点的text控件列表
    QList<QWidget*> lines; // 线，0:time左边，1~..为控件左边（已废弃，使用 paintEvent 重绘线条）
};

#endif // TIMEBUCKET_H
