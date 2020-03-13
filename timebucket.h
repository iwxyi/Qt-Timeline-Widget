#ifndef TIMEBUCKET_H
#define TIMEBUCKET_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QDebug>
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
    TimelineTimeLabel* time_widget; // 时间点的线
    QList<TimelineTextLabel*> text_widgets; // 这个时间点的text控件列表
    QList<QWidget*> lines; // 线，0:time左边，1~..为控件左边（已废弃，使用 paintEvent 重绘线条）

    int horizone_spacing = 30;
    int vertical_spacing = 30;
};

#endif // TIMEBUCKET_H
