#include "timelinebucket.h"

TimelineBucket::TimelineBucket(QWidget *parent) : QWidget(parent)
{
    setAcceptDrops(true);

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

    emit signalSizeHintChanged(getSuitableSize());
}

void TimelineBucket::setText(QString text)
{
    this->clearText();
    addTextWidget(text);

    emit signalSizeHintChanged(getSuitableSize());
}

void TimelineBucket::setText(QStringList texts)
{
    this->clearText();
    foreach (QString text, texts)
    {
        addTextWidget(text);
    }

    emit signalSizeHintChanged(getSuitableSize());
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
    if (processDropEvent(event))
        event->accept();
    QWidget::dragEnterEvent(event);
}

void TimelineBucket::dragMoveEvent(QDragMoveEvent *event)
{
    if (processDropEvent(event))
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

    }
    else // 外部拖拽，交换顺序/移动数据
    {

    }

    QWidget::dropEvent(event);
}

bool TimelineBucket::processDropEvent(QDropEvent *event)
{
    const QMimeData* mime = event->mimeData();
    if (mime->hasFormat(TIMELINE_BUCKET_MIME_KEY))
    {
        TimelineBucket* bucket = reinterpret_cast<TimelineBucket*>(mime->data(TIMELINE_BUCKET_MIME_KEY).toInt());
        if (bucket)
        {
            // 根据位置，修改不同的位置


            return true;
        }
        return false;
    }
    else if (mime->hasFormat("TIMELINE_TEXT") || mime->hasFormat("TIMELINE_TIME"))
    {

    }
    return false;
}
