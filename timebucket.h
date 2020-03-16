#ifndef TIMEBUCKET_H
#define TIMEBUCKET_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QDebug>
#include "timelinetimelabel.h"
#include "timelinetextlabel.h"
#include "timelineleadingdot.h"

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

    void clearText();

    void setTimeLabelWidth(int w);
    QSize getSuitableSize();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void addTextWidget(QString text = "");

signals:
    void signalSizeHintChanged(QSize size);

public slots:

private:
    QHBoxLayout* hlayout;
    TimelineLeadingDot* leading_dot; // 开头的小点
    TimelineTimeLabel* time_widget; // 时间点的线
    QList<TimelineTextLabel*> text_widgets; // 这个时间点的text控件列表
    QSpacerItem* time_spacer; // 时间左边的space
    QList<QSpacerItem*> text_spacers; // Text控件左边的空白

    int padding_left = 10; // 最左边开始的位置
    int leading_dot_radius = 5; // 开头小圆点的半径
    int dot_time_spacing  = 30; // 时间和小圆点的距离
    int horizone_spacing = 30; // 每个text之间的间隔
    int vertical_spacing = 30; // 竖向两个时间段的固定距离
};

#endif // TIMEBUCKET_H
