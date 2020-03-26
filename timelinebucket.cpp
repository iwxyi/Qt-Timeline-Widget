#include "timelinebucket.h"

TimelineBucket::TimelineBucket(QWidget *parent) : QWidget(parent)
{
    setAcceptDrops(true);

    selecting = false;

    initView();
}

void TimelineBucket::initView()
{
    hlayout = new QHBoxLayout(this);
    leading_dot = new TimelineLeadingDot(this);
    time_spacer = new QSpacerItem(dot_time_spacing, 0);
    time_widget = new TimelineTimeLabel(this);

    leading_dot->setRadius(5);
    leading_dot->setColor(Qt::blue);

    hlayout->addSpacing(padding_left);
    hlayout->addWidget(leading_dot);
    hlayout->addSpacerItem(time_spacer);
    hlayout->addWidget(time_widget);

    setLayout(hlayout);
    hlayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    connect(time_widget, &TimelineTimeLabel::signalClicked, this, [=] {
        emit signalTimeWidgetClicked(time_widget);
    });
    connect(time_widget, &TimelineTimeLabel::signalDoubleClicked, this, [=] {
        emit signalTimeWidgetDoubleClicked(time_widget);
    });
}

void TimelineBucket::setVerticalIndex(int index)
{
    vertical_index = index;
}

void TimelineBucket::setTime(QString time)
{
    time_widget->setText(time);
    time_widget->adjustSize();
    time_widget->setScaledContents(true);
}

void TimelineBucket::setText(int index, QString text)
{
    if (index < 0 || index > text_widgets.size()) // 错误
        return ;
    if (index == text_widgets.size()) // 加一个
        return addTextWidget(text);

    // 设置内容
    text_widgets.at(index)->setText(text);

    adjustBucketSize();
}

void TimelineBucket::setText(QString text)
{
    this->clearText();
    addTextWidget(text);

    adjustBucketSize();
}

void TimelineBucket::setText(QStringList texts)
{
    this->clearText();
    foreach (QString text, texts)
    {
        addTextWidget(text);
    }

    adjustBucketSize();
}

void TimelineBucket::addTextWidget(QString text)
{
    TimelineTextLabel* label = new TimelineTextLabel(this);
    label->setText(text);
    text_widgets.append(label);

    QSpacerItem* spacer = new QSpacerItem(horizone_spacing, 0);
    text_spacers.append(spacer);

    hlayout->addSpacerItem(spacer);
    hlayout->addWidget(label);

    connect(label, &TimelineTextLabel::signalClicked, this, [=] {
        emit signalTextWidgetClicked(label);
    });
    connect(label, &TimelineTextLabel::signalDoubleClicked, this, [=] {
        emit signalTextWidgetDoubleClicked(label);
    });
    connect(label, &TimelineTextLabel::signalInsertLeft, this, [=]{
        actionInsertLeft(label);
    });
    connect(label, &TimelineTextLabel::signalInsertRight, this, [=]{
        actionInsertRight(label);
    });
    connect(label, &TimelineTextLabel::signalDelete, this, [=]{
        actionDelete(label);
    });
    connect(label, &TimelineTextLabel::signalDraggedToOut, this, [=]{
        int index = text_widgets.indexOf(label);
        hlayout->removeItem(text_spacers.at(index));
        hlayout->removeWidget(label);
        delete text_spacers.takeAt(index);
        text_widgets.removeAt(index);

    });
}

void TimelineBucket::actionInsertLeft(TimelineTextLabel *label)
{
    int index = text_widgets.indexOf(label);
    if (index == -1)
        return ;


}

void TimelineBucket::actionInsertRight(TimelineTextLabel *label)
{
    int index = text_widgets.indexOf(label);
    if (index == -1)
        return ;



}

void TimelineBucket::actionDelete(TimelineTextLabel *label)
{
    int index = text_widgets.indexOf(label);
    if (index == -1)
        return ;


}

void TimelineBucket::actionMoveTextLabel(int from, int to)
{

}

void TimelineBucket::clearText()
{
    foreach (TimelineTextLabel* label, text_widgets) {
        label->deleteLater();
    }
    text_widgets.clear();
}

void TimelineBucket::setTimeLabelWidth(int w)
{
    time_widget->setFixedWidth(w);
}

void TimelineBucket::adjustBucketSize()
{
    QSize size = getSuitableSize();
    setMinimumSize(size);
    emit signalSizeHintChanged(size);
}

/**
 * 根据里面的内容，获取对应合适的宽度和高度
 */
QSize TimelineBucket::getSuitableSize()
{
    int sw = horizone_spacing + time_widget->width();
    int sh = 0;
    for (int i = 0; i < text_widgets.size(); ++i)
    {
        if (sh < text_widgets.at(i)->height())
            sh = text_widgets.at(i)->height();
        sw += horizone_spacing + text_widgets.at(i)->width();
    }
    sw += hlayout->margin() * 2 + hlayout->spacing() * text_widgets.size();
    sh += hlayout->margin() * 2;

    return QSize(sw, sh+vertical_spacing);
}

void TimelineBucket::adjustTextsPositions(int start)
{
    int left = (start-1)>=0 ? text_spacers.at(start-1)->geometry().left() : time_widget->geometry().right()+1;
    int end = text_widgets.size();
    for (int i = start; i < end; i++)
    {
        // 自动调整的，无需手动干预
    }
}

void TimelineBucket::adjustTextsPositionsWithAnimation(int start, int end)
{
    int left = (start-1)>=0 ? text_spacers.at(start-1)->geometry().left() : time_widget->geometry().right()+1;
    if (end == -1)
        end = text_widgets.size();
    for (int i = start; i < end; i++)
    {
        auto spacer = text_spacers.at(i);
        auto widget = text_widgets.at(i);
        QPropertyAnimation* ani = new QPropertyAnimation(widget, "pos");
        ani->setStartValue(widget->pos());
        ani->setEndValue(QPoint(left+spacer->geometry().width(), widget->pos().y()));
        ani->setDuration(300);
        ani->setEasingCurve(QEasingCurve::OutQuart);
        connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
        ani->start();
    }
}

bool TimelineBucket::isSelected()
{
    return selecting;
}

void TimelineBucket::setSelected(bool select)
{
    this->selecting = select;
}

void TimelineBucket::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setPen(QPen(QColor(0, 0, 200, 126)));
    int y = leading_dot->geometry().center().y();

    // 画时间轴的线
    int left = padding_left + leading_dot_radius*2;
    int right = time_widget->geometry().left();
    painter.drawLine(QPoint(left, y), QPoint(right, y));

    // 每个控件之前画线条
    for (int i = 0; i < text_widgets.size(); ++i)
    {
        if (i)
            left = text_widgets.at(i-1)->geometry().right();
        else
            left = time_widget->geometry().right();
        right = text_widgets.at(i)->geometry().left();
        painter.drawLine(QPoint(left, y), QPoint(right, y));
    }

    // 画和下一个连接的线条
    if (vertical_index)
    {
        painter.drawLine(QPoint(leading_dot->geometry().center().x(), 0), QPoint(leading_dot->geometry().center().x(), height()));
    }
    else // 第一行
    {
        painter.drawLine(leading_dot->geometry().center(), QPoint(leading_dot->geometry().center().x(), height()));
    }
}

void TimelineBucket::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
}

void TimelineBucket::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
}

void TimelineBucket::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        press_pos = event->pos();
    }

    QWidget::mousePressEvent(event);
}

void TimelineBucket::mouseReleaseEvent(QMouseEvent *event)
{
    if ((press_pos-event->pos()).manhattanLength() < QApplication::startDragDistance())
    {
        emit signalBucketWidgetClicked();
    }

    QWidget::mouseReleaseEvent(event);
}

void TimelineBucket::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
}

void TimelineBucket::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) // 左键拖拽
    {
        if ((event->pos() - press_pos).manhattanLength() >= QApplication::startDragDistance())
        {
            QMimeData* mime = new QMimeData();
            mime->setData(TIMELINE_BUCKET_MIME_KEY, QString::number(reinterpret_cast<int>(this)).toUtf8());
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

    QWidget::mouseMoveEvent(event);
}

/**
 * 拖拽的内容进入事件
 */
void TimelineBucket::dragEnterEvent(QDragEnterEvent *event)
{
    if (canDropMimeData(event))
        event->accept();
    QWidget::dragEnterEvent(event);
}

void TimelineBucket::dragMoveEvent(QDragMoveEvent *event)
{
    if (canDropMimeData(event))
        event->accept();

    QWidget::dragMoveEvent(event);
}

void TimelineBucket::dragLeaveEvent(QDragLeaveEvent *event)
{
    QWidget::dragLeaveEvent(event);
}

/**
 * 拖拽时鼠标松开事件
 */
void TimelineBucket::dropEvent(QDropEvent *event)
{
    const QMimeData* mime = event->mimeData();
    if (event->source() == this) // 内部拖拽
    {
        ; // 经常触发的自己拖拽给自己，无视
    }
    else // 外部拖拽，交换顺序/移动数据
    {
        if (mime->hasFormat(TIMELINE_BUCKET_MIME_KEY)) // 上下交换行
        {
            TimelineBucket* bucket = reinterpret_cast<TimelineBucket*>(mime->data(TIMELINE_BUCKET_MIME_KEY).toInt());
            emit signalDroppedAndMoved(bucket);
        }
        else if (mime->hasFormat(TIMELINE_TEXT_MIME_KEY)) // 左右交换文字
        {
            TimelineTextLabel* label = reinterpret_cast<TimelineTextLabel*>(mime->data(TIMELINE_TEXT_MIME_KEY).toInt());

            // 获取拖拽到目标的索引
            int to_index = text_widgets.size(); // 默认是到最后面
            QPoint pos = event->pos();
            for (int i = 0; i < text_widgets.size(); i++)
            {
                if (text_widgets.at(i)->geometry().left() >= pos.x()) // 移到这个 label 的后面
                {
                    to_index = i;
                    break;
                }
            }

            // 放下的位置的索引
            int layout_index = 0;
            if (to_index == text_widgets.size()) // 最后面
                layout_index = hlayout->count();
            else
                layout_index = hlayout->indexOf(text_widgets.at(to_index)); // 获取前面空白的位置

            // 获取被拖拽的目标
            int from_index = text_widgets.indexOf(label);
            if (from_index == -1) // 外面拖进来的
            {
                emit label->signalDraggedToOut(); // 从父类那里删掉
                label->setParent(this);

                auto spacer = new QSpacerItem(horizone_spacing, 1);
                text_spacers.insert(to_index, spacer);
                text_widgets.insert(to_index, label);

                hlayout->insertWidget(layout_index, label);
                hlayout->insertItem(layout_index, spacer);
            }
            else // 自己的，删掉旧的
            {
                auto spacer = text_spacers.at(from_index);
                auto widget = text_widgets.at(from_index);

                hlayout->removeItem(spacer);
                hlayout->removeWidget(widget);
                text_spacers.removeAt(from_index);
                text_widgets.removeAt(from_index);

                text_spacers.insert(to_index, spacer);
                text_widgets.insert(to_index, widget);

                hlayout->insertWidget(layout_index, label);
                hlayout->insertItem(layout_index, spacer);
            }
        }
    }

    QWidget::dropEvent(event);
}

bool TimelineBucket::canDropMimeData(QDropEvent *event)
{
    const QMimeData* mime = event->mimeData();
    if (mime->hasFormat(TIMELINE_BUCKET_MIME_KEY)) // 整行拖拽
    {
        TimelineBucket* bucket = reinterpret_cast<TimelineBucket*>(mime->data(TIMELINE_BUCKET_MIME_KEY).toInt());
        if (bucket)
        {
            return true;
        }
    }
    else if (mime->hasFormat(TIMELINE_TEXT_MIME_KEY)) // 单个TextWidget拖拽
    {
        return true;
    }
    return false;
}
