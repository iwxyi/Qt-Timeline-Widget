#include "timelineleadingdot.h"

TimelineLeadingDot::TimelineLeadingDot(QWidget *parent) : QWidget(parent), bg_color(Qt::blue)
{

}

/**
 * 设置背景颜色
 */
void TimelineLeadingDot::setColor(QColor c)
{
    this->bg_color = c;
    update();
}

/**
 * 设置半径，即width和height都是r*2
 */
void TimelineLeadingDot::setRadius(int r)
{
    setFixedSize(r*2, r*2);
}

void TimelineLeadingDot::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    // 画好看的点
    QPainter painter(this);
    QPainterPath path;
    painter.setRenderHint(QPainter::Antialiasing, true);
    path.addEllipse(0,0,width(), height());
    painter.fillPath(path, bg_color);
}
