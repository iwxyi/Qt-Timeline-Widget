#ifndef TIMELINEBUCKET_H
#define TIMELINEBUCKET_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QDebug>
#include <QMimeData>
#include <QDrag>
#include "timelinetimelabel.h"
#include "timelinetextlabel.h"
#include "timelineleadingdot.h"

#define TIMELINE_BUCKET_MIME_KEY "TIMELINE_BUCKET"

class TimelineBucket : public QWidget
{
    Q_OBJECT
public:
    TimelineBucket(QWidget *parent = nullptr);
    void initView();

    void setVerticalIndex(int index);
    void setTime(QString time);
    void setText(int index, QString text);
    void setText(QString text);
    void setText(QStringList texts);

    void clearText();

    void setTimeLabelWidth(int w);
    QSize getSuitableSize();

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void addTextWidget(QString text = "");
    bool processDropEvent(QDropEvent* event);

signals:
    void signalSizeHintChanged(QSize size);
    void signalTimeWidgetClicked(TimelineTimeLabel* label);
    void signalTextWidgetClicked(TimelineTextLabel* label);
    void signalTimeWidgetDoubleClicked(TimelineTimeLabel* label);
    void signalTextWidgetDoubleClicked(TimelineTextLabel* label);

public slots:
    void actionInsertLeft(TimelineTextLabel* label);
    void actionInsertRight(TimelineTextLabel* label);
    void actionDelete(TimelineTextLabel* label);

private:
    QHBoxLayout* hlayout;
    TimelineLeadingDot* leading_dot; // 开头的小点
    QSpacerItem* time_spacer; // 时间左边的space
    TimelineTimeLabel* time_widget; // 时间点的线
    QList<QSpacerItem*> text_spacers; // Text控件左边的空白
    QList<TimelineTextLabel*> text_widgets; // 这个时间点的text控件列表

    int vertical_index; // 在整个时间轴里面的下标

    int padding_left = 10; // 最左边开始的位置
    int leading_dot_radius = 5; // 开头小圆点的半径
    int dot_time_spacing  = 30; // 时间和小圆点的距离
    int horizone_spacing = 30; // 每个text之间的间隔
    int vertical_spacing = 30; // 竖向两个时间段的固定距离

    QPoint press_pos;
};

#endif // TIMELINEBUCKET_H
