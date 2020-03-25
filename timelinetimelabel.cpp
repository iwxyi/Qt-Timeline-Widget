#include "timelinetimelabel.h"

TimelineTimeLabel::TimelineTimeLabel(QWidget *parent) : QLabel(parent)
{
    setObjectName("TimelineTimeLabel");
}

void TimelineTimeLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        press_pos = event->pos();
    QLabel::mousePressEvent(event);
}

void TimelineTimeLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if ((press_pos-event->pos()).manhattanLength() < QApplication::startDragDistance())
        {
            emit signalClicked();
        }
    }
    QLabel::mouseReleaseEvent(event);
}
