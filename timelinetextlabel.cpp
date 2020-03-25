#include "timelinetextlabel.h"

TimelineTextLabel::TimelineTextLabel(QWidget *parent) : QLabel(parent)
{
    setObjectName("TimelineTextLabel");

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested (const QPoint&)),this,SLOT(slotMenuShowed(const QPoint&)));
}

void TimelineTextLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        press_pos = event->pos();
    QLabel::mousePressEvent(event);
}

void TimelineTextLabel::mouseReleaseEvent(QMouseEvent *event)
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

void TimelineTextLabel::slotMenuShowed(const QPoint &pos)
{

}
