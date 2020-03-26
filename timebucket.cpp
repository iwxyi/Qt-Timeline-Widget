#include "timebucket.h"

TimeBucket::TimeBucket(QWidget *parent) : QWidget(parent)
{
    setAcceptDrops(true);

    initView();
}

void TimeBucket::initView()
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

void TimeBucket::setVerticalIndex(int index)
{
    vertical_index = index;
}

void TimeBucket::setTime(QString time)
{
    time_widget->setText(time);
    time_widget->adjustSize();
    time_widget->setScaledContents(true);
}

void TimeBucket::setText(int index, QString text)
{
    if (index < 0 || index > text_widgets.size()) // 错误
        return ;
    if (index == text_widgets.size()) // 加一个
        return addTextWidget(text);

    // 设置内容
    text_widgets.at(index)->setText(text);

    emit signalSizeHintChanged(getSuitableSize());
}

void TimeBucket::setText(QString text)
{
    this->clearText();
    addTextWidget(text);

    emit signalSizeHintChanged(getSuitableSize());
}

void TimeBucket::setText(QStringList texts)
{
    this->clearText();
    foreach (QString text, texts)
    {
        addTextWidget(text);
    }

    emit signalSizeHintChanged(getSuitableSize());
}

void TimeBucket::addTextWidget(QString text)
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

void TimeBucket::actionInsertLeft(TimelineTextLabel *label)
{
    int index = text_widgets.indexOf(label);
    if (index == -1)
        return ;


}

void TimeBucket::actionInsertRight(TimelineTextLabel *label)
{
    int index = text_widgets.indexOf(label);
    if (index == -1)
        return ;



}

void TimeBucket::actionDelete(TimelineTextLabel *label)
{
    int index = text_widgets.indexOf(label);
    if (index == -1)
        return ;


}

void TimeBucket::clearText()
{
    foreach (TimelineTextLabel* label, text_widgets) {
        label->deleteLater();
    }
    text_widgets.clear();
}

void TimeBucket::setTimeLabelWidth(int w)
{
    time_widget->setFixedWidth(w);
}

/**
 * 根据里面的内容，获取对应合适的宽度和高度
 */
QSize TimeBucket::getSuitableSize()
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

void TimeBucket::paintEvent(QPaintEvent *event)
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

void TimeBucket::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
}

void TimeBucket::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
}

void TimeBucket::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}

void TimeBucket::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
}

void TimeBucket::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
}

void TimeBucket::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) // 左键拖拽
    {
        if ((event->pos() - press_pos).manhattanLength() >= QApplication::startDragDistance())
        {
            QMimeData* mime = new QMimeData();
            mime->setData("BUCKET", QString::number(reinterpret_cast<int>(this)).toUtf8());
            QDrag* drag = new QDrag(this);
            drag->setMimeData(mime);
            drag->exec(Qt::MoveAction);
            emit signalStartDrag();
        }
    }

    QWidget::mouseMoveEvent(event);
}

/**
 * 拖拽的内容进入事件
 */
void TimeBucket::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "TimeBucket::dragEnterEvent";
    const QMimeData* mime = event->mimeData();
    if (mime->hasFormat("BUCKET"))
    {
        TimeBucket* bucket = reinterpret_cast<TimeBucket*>(mime->data("BUCKET").toInt());
        if (bucket)
        {
            qDebug() << "accept";
            event->accept();
        }
    }
}

void TimeBucket::dragLeaveEvent(QDragLeaveEvent *event)
{
    qDebug() << "TimeBucket::dragLeaveEvent";
    QWidget::dragLeaveEvent(event);
}

/**
 * 拖拽时鼠标松开事件
 */
void TimeBucket::dropEvent(QDropEvent *event)
{
    qDebug() << "TimeBucket::dropEvent";
    const QMimeData* mime = event->mimeData();
    if (mime->hasFormat("BUCKET"))
    {

    }
    event->ignore();
    QWidget::dropEvent(event);
}
