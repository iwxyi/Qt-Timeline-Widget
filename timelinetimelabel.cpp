#include "timelinetimelabel.h"

TimelineTimeLabel::TimelineTimeLabel(QWidget *parent) : QLabel(parent)
{
    setObjectName("TimelineTimeLabel");

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested (const QPoint&)),this,SLOT(slotMenuShowed(const QPoint&)));
}

void TimelineTimeLabel::adjustSize(bool notify)
{
    QSize old_size = size();
    QLabel::adjustSize();
    if (notify && old_size != size())
        emit signalSizeChanged(size());
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

void TimelineTimeLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit signalDoubleClicked();
    }

    QLabel::mouseDoubleClickEvent(event);
}

void TimelineTimeLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {

    }

    QLabel::mouseMoveEvent(event);
}

void TimelineTimeLabel::slotMenuShowed(const QPoint& pos)
{
    QMenu* menu = new QMenu("菜单", this);
    QAction* current_time_action = new QAction("设为当前日期", this);
    menu->addAction(current_time_action);

    connect(current_time_action, &QAction::triggered, this, [=] {
        QString str = QDate::currentDate().toString("yyyy.MM.dd");
        setText(str);
        adjustSize();
    });

    menu->exec(QCursor::pos());
}
