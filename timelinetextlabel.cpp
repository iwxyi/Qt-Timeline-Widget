#include "timelinetextlabel.h"

TimelineTextLabel::TimelineTextLabel(QWidget *parent) : QLabel(parent)
{
    setObjectName("TimelineTextLabel");

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested (const QPoint&)),this,SLOT(slotMenuShowed(const QPoint&)));
}

TimelineTextLabel::TimelineTextLabel(TimelineTextLabel *&another, QWidget *parent) : TimelineTextLabel(parent)
{
    setText(another->text());
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

void TimelineTextLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit signalDoubleClicked();
    }

    QLabel::mouseDoubleClickEvent(event);
}

void TimelineTextLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) // 左键拖拽
    {
        if ((event->pos() - press_pos).manhattanLength() >= QApplication::startDragDistance())
        {
            QMimeData* mime = new QMimeData();
            mime->setData(TIMELINE_TEXT_MIME_KEY, QString::number(reinterpret_cast<int>(this)).toUtf8());
            QDrag* drag = new QDrag(this);
            drag->setMimeData(mime);
            drag->setHotSpot(event->pos()); // 好像没什么用
            QPixmap pixmap(size());
            pixmap.fill(Qt::transparent);
            render(&pixmap);
            drag->setPixmap(pixmap);
            drag->exec(Qt::MoveAction);
            return event->accept();
        }
    }

    QLabel::mouseMoveEvent(event);
}

void TimelineTextLabel::slotMenuShowed(const QPoint &pos)
{
    QMenu* menu = new QMenu("菜单", this);
    QAction* insert_left_action = new QAction("插入左边", this);
    QAction* insert_right_action = new QAction("插入右边", this);
    QAction* delete_action = new QAction("删除", this);
    QAction* copy_text_action = new QAction("复制文字", this);
    menu->addAction(insert_left_action);
    menu->addAction(insert_right_action);
    menu->addAction(delete_action);
    menu->addAction(copy_text_action);

    connect(insert_left_action, &QAction::triggered, this, [=] {
        emit signalInsertLeft();
    });
    connect(insert_right_action, &QAction::triggered, this, [=] {
        emit signalInsertRight();
    });
    connect(delete_action, &QAction::triggered, this, [=] {
        emit signalDelete();
    });
    connect(insert_left_action, &QAction::triggered, this, [=] {
        QClipboard* clip = QApplication::clipboard();
        clip->setText(text());
    });

    menu->exec(QCursor::pos());
}
