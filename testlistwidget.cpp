#include "testlistwidget.h"

TestListWidget::TestListWidget(QWidget *parent) : QListWidget(parent)
{
    setDragEnabled(true);
    setAcceptDrops(true);
    setDragDropOverwriteMode(true);

}

void TestListWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        press_pos = event->pos();
    }
    QListWidget::mousePressEvent(event);
}

void TestListWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) // 左键拖拽
    {
        if ((event->pos() - press_pos).manhattanLength() >= QApplication::startDragDistance())
        {
            QMimeData* mime = new QMimeData();
            QDrag* drag = new QDrag(this);
            drag->setMimeData(mime);
            drag->exec(Qt::MoveAction);
            return ;
        }
    }
    QListWidget::mouseMoveEvent(event);
}

void TestListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "接收";
    event->acceptProposedAction();
    event->accept();

//    QListWidget::dragEnterEvent(event);
}
