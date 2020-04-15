#include "timelinebucket.h"

TimelineBucket::TimelineBucket(QWidget *parent) : QWidget(parent)
{
    setAcceptDrops(true);

    selecting = false;
    current_index = -1;

    initView();
}

void TimelineBucket::initView()
{
    leading_dot = new TimelineLeadingDot(this);
    time_widget = new TimelineTimeLabel(this);

    leading_dot->setRadius(5);
    leading_dot->setColor(Qt::blue);

    adjustWidgetsPositions();

    connect(time_widget, &TimelineTimeLabel::signalClicked, this, [=] {
        emit signalTimeWidgetClicked(time_widget);
    });
    connect(time_widget, &TimelineTimeLabel::signalDoubleClicked, this, [=] {
        emit signalTimeWidgetDoubleClicked(time_widget);
    });
    connect(time_widget, &TimelineTimeLabel::signalSizeChanged, this, [=](QSize) { // 一般由 setTime 触发
        adjustWidgetsPositionsWithAnimation();
    });
}

void TimelineBucket::setVerticalIndex(int index)
{
    vertical_index = index;
}

void TimelineBucket::setTime(QString time)
{
    time_widget->setText(time);
    time_widget->setScaledContents(true);
    time_widget->adjustSize();
}

void TimelineBucket::setText(int index, QString text)
{
    if (index < 0 || index > text_widgets.size()) // 错误
        return ;
    if (index == text_widgets.size()) // 加一个
    {
        addTextWidget(text);
        return ;
    }

    // 设置内容
    text_widgets.at(index)->setText(text);
    text_widgets.at(index)->adjustSize();
}

void TimelineBucket::setText(QString text)
{
    clearText();
    addTextWidget(text);

    adjustWidgetsSize();
    adjustWidgetsPositions();
}

void TimelineBucket::setText(QStringList texts)
{
    clearText();
    foreach (QString text, texts)
    {
        addTextWidget(text);
    }

    adjustWidgetsSize();
    adjustWidgetsPositions();
}

TimelineTextLabel* TimelineBucket::insertText(int index, QString text)
{
    auto widget = insertTextWidget(text, index);
    adjustWidgetsPositionsWithAnimation();
    return widget;
}

QString TimelineBucket::getText(int index)
{
    if (index < 0 || index >= text_widgets.size())
        return "";
    return text_widgets.at(index)->text();
}

TimelineTextLabel* TimelineBucket::addTextWidget(QString text)
{
    return insertTextWidget(text, -1);
}

TimelineTextLabel* TimelineBucket::insertTextWidget(QString text, int index)
{
    TimelineTextLabel* label = new TimelineTextLabel(this);
    label->setText(text);
    label->adjustSize(false);

    if (index > -1)
    {
        if (index > 0)
            label->move(text_widgets.at(index-1)->geometry().topRight());
        else if (index < text_widgets.size())
            label->move(text_widgets.at(index)->geometry().topLeft());
        text_widgets.insert(index, label);
    }
    else // index <= -1
    {
        text_widgets.append(label);
        if (text_widgets.size()>1)
        {
            label->move(text_widgets.at(text_widgets.size()-2)->geometry().topRight());
        }
        else
            label->move(time_widget->geometry().topRight());
    }
    label->show();

    connectWidgetEvent(label);
    return label;
}

void TimelineBucket::connectWidgetEvent(TimelineTextLabel *label)
{
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
        text_widgets.takeAt(index)->deleteLater();
        adjustBucketSize();
        adjustWidgetsPositionsWithAnimation();
    });
    connect(label, &TimelineTextLabel::signalSizeChanged, this, [=](QSize) {
        // 一般由 setText(index, text) 触发，调整内容时文本框大小改变
        adjustBucketSize();
        adjustWidgetsPositionsWithAnimation();
    });
}

void TimelineBucket::actionInsertLeft(TimelineTextLabel *label)
{
    int index = text_widgets.indexOf(label);
    if (index == -1)
        return ;

    insertTextWidget("", index);
    adjustWidgetsPositionsWithAnimation(index);
}

void TimelineBucket::actionInsertRight(TimelineTextLabel *label)
{
    int index = text_widgets.indexOf(label);
    if (index == -1)
        return ;

    insertTextWidget("", index+1);
    adjustWidgetsPositionsWithAnimation(index+1);
}

void TimelineBucket::actionDelete(TimelineTextLabel *label)
{
    int index = text_widgets.indexOf(label);
    if (index == -1)
        return ;
    text_widgets.takeAt(index)->deleteLater();

    adjustWidgetsPositionsWithAnimation(index);
}

void TimelineBucket::actionMoveTextLabel(int from, int to)
{
    auto widget = text_widgets.takeAt(from);
    text_widgets.insert(to, widget);

    adjustWidgetsPositionsWithAnimation(qMin(from, to));
}

void TimelineBucket::clearText()
{
    foreach (TimelineTextLabel* label, text_widgets) {
        label->deleteLater();
    }
    text_widgets.clear();
}

QString TimelineBucket::toString(QString time_format, QString para_split)
{
    QString result = QString(time_format).arg(time_widget->text());
    foreach (auto widget, text_widgets)
    {
        result += para_split + widget->text();
    }
    return result;
}

void TimelineBucket::setTimeLabelWidth(int w)
{
    time_widget->setFixedWidth(w);
}

/**
 * 修改控件尺寸（不包含位置）
 */
void TimelineBucket::adjustWidgetsSize()
{
    time_widget->adjustSize(false);
    for (int i = 0; i < text_widgets.size(); i++)
        text_widgets.at(i)->adjustSize(false);
}

/**
 * 根据控件的大小，修改整个外框的大小
 */
void TimelineBucket::adjustBucketSize()
{
    QSize size = getSuitableSize();
    setMinimumSize(size);
//    resize(size);
    setFixedHeight(size.height());
    emit signalSizeHintChanged(size);
}

/**
 * 根据控件大小，获取对应合适的宽度和高度
 */
QSize TimelineBucket::getSuitableSize()
{
    int sw = padding_left + leading_dot_radius*2 + time_widget->width() + dot_time_spacing;
    int sh = time_widget->height();
    for (int i = 0; i < text_widgets.size(); ++i)
    {
        if (sh < text_widgets.at(i)->height())
            sh = text_widgets.at(i)->height();
        sw += text_widgets.at(i)->width();
    }
    sw += horizone_spacing * (text_widgets.size()+1);
    sh += vertical_spacing;
    return QSize(sw, sh);
}

/**
 * 移动所有控件的位置（不改变大小）
 */
void TimelineBucket::adjustWidgetsPositions(int start)
{
    QSize old_size = size();
    QSize size = getSuitableSize();
    if (size != old_size)
    {
        setMinimumSize(size);
        setFixedHeight(size.height());
        emit signalSizeHintChanged(size);
    }
    int mid_y = height() / 2;
    leading_dot->move(padding_left, mid_y-leading_dot_radius);
    time_widget->move(leading_dot->geometry().right() + dot_time_spacing, mid_y - time_widget->height()/2);

    int left = (start-1)>=0 ? text_widgets.at(start-1)->geometry().right() : time_widget->geometry().right();
    int end = text_widgets.size();
    for (int i = start; i < end; i++)
    {
        TimelineTextLabel* label = text_widgets.at(i);
        left += horizone_spacing;
        label->move(left, mid_y - label->height() / 2);
        left += label->width();
    }
}

/**
 * 调整所有控件的位置，包含动画
 */
void TimelineBucket::adjustWidgetsPositionsWithAnimation(int start, int end)
{
    QSize old_size = size();
    QSize size = getSuitableSize();
    if (size != old_size)
    {
        setMinimumSize(size);
        setFixedHeight(size.height());
        emit signalSizeHintChanged(size);
    }
    int mid_y = height() / 2;
    leading_dot->move(padding_left, mid_y-leading_dot_radius);
    time_widget->move(leading_dot->geometry().right() + dot_time_spacing, mid_y - time_widget->height()/2);

    int left = (start-1)>=0 ? text_widgets.at(start-1)->geometry().right() : time_widget->geometry().right();
    if (end == -1)
        end = text_widgets.size();
    bool firsted = false;
    for (int i = start; i < end; i++)
    {
        TimelineTextLabel* label = text_widgets.at(i);
        left += horizone_spacing;

        QPropertyAnimation* ani = new QPropertyAnimation(label, "pos");
        ani->setStartValue(label->pos());
        ani->setEndValue(QPoint(left, mid_y - label->height()/2));
        ani->setDuration(300);
        ani->setEasingCurve(QEasingCurve::OutQuart);
        connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
        ani->start();
        if (!firsted)
        {
            // 在第一个控件的动画上连接信号槽，实时重绘
            connect(ani, SIGNAL(valueChanged(const QVariant &)), this, SLOT(update()));
            firsted = true;
        }

        left += label->width();
    }
}

bool TimelineBucket::isSelected()
{
    return selecting;
}

void TimelineBucket::setSelected(bool select)
{
    this->selecting = select;
    repaint();

    QPropertyAnimation* ani = new QPropertyAnimation(this, "water_prop");
    ani->setStartValue(water_prop);
    ani->setEndValue( select ? 100 : 0 );
    ani->setEasingCurve(QEasingCurve::OutQuad);
    ani->setDuration(300);
    connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
    ani->start();
}

void TimelineBucket::setPressPos(QPoint pos)
{
    water_source = pos;
}

void TimelineBucket::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    adjustWidgetsSize();
    adjustWidgetsPositions();
}

void TimelineBucket::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setPen(QPen(QColor(0, 0, 200, 126)));
    int y = leading_dot->geometry().center().y();

    // 画选中情况
    if (water_prop == 0)
    {

    }
    else if (water_prop == 100)
    {
        painter.fillRect(QRect(0,0,width(),height()), select_color);
    }
    else // 1 ~ 99
    {
        if (!selecting || water_source == QPoint(-1, -1)) // 不知位置，直接渐变
        {
            QColor c = select_color;
            c.setAlpha(select_color.alpha() * water_prop / 100);
            painter.fillRect(QRect(0,0,width(),height()), c);
        }
        else // 画水波纹动画
        {
            int w_max = qMax(water_source.x(), width() - water_source.y());
            int h_max = qMax(water_source.y(), height() - water_source.y());
            int r_max = qMax(w_max, h_max);
            int radius = r_max * water_prop / 100;
            QPainterPath path;
            path.addEllipse(water_source.x() - radius, water_source.y() - radius, radius*2, radius*2);
            painter.fillPath(path, select_color);
        }
    }

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
        water_source = press_pos = event->pos();
        if (water_prop != 100) // 如果已经选中了，就不重新刷新了
            water_prop = 0;
        emit signalBucketWidgetPressed();
    }
    else if (event->button() == Qt::RightButton)
    {
        if (!isSelected())
            emit signalBucketWidgetPressed();
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
            mime->setText(toString());
            QDrag* drag = new QDrag(this);
            drag->setMimeData(mime);
            drag->setHotSpot(event->pos()); // 设置拖拽缩略图与拖拽的点的相对位移
            QPixmap pixmap(getSuitableSize());

            {
                water_prop = 0; // 画未选中的
                repaint();
                pixmap.fill(Qt::transparent);
                render(&pixmap);

                // 保存临时文件
                QString path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
                QString time = time_widget->text();
                time.replace("\\", "").replace("/", "").replace(":", "").replace("*", "").replace("?", "").replace("\"", "")
                        .replace("<", "").replace(">", "").replace("|", "");
                if (time.isEmpty())
                    time = "未定";
                path += "/"+time+".png";
                pixmap.save(path);
                mime->setUrls(QList<QUrl>{QUrl("file:///" + path)});
            }

            {
                water_prop = 100; // 画选中的
                repaint();
                pixmap.fill(Qt::transparent);
                render(&pixmap);
                drag->setPixmap(pixmap);
            }

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
        // 经常触发的自己拖拽给自己，无视
    }
    else // 外部拖拽，交换顺序/移动数据
    {
        if (mime->hasFormat(TIMELINE_BUCKET_MIME_KEY)) // 上下交换行
        {
            TimelineBucket* bucket = reinterpret_cast<TimelineBucket*>(mime->data(TIMELINE_BUCKET_MIME_KEY).toInt());
            emit signalDroppedAndMoved(bucket);
        }
        else if (mime->hasFormat(TIMELINE_TEXT_MIME_KEY)) // 左右或者和其他bucket交换文字
        {
            TimelineTextLabel* label = reinterpret_cast<TimelineTextLabel*>(mime->data(TIMELINE_TEXT_MIME_KEY).toInt());

            // 获取拖拽到目标的索引
            int to_index = text_widgets.size(); // 默认是到最后面
            QPoint pos = event->pos();
            for (int i = 0; i < text_widgets.size(); i++)
            {
                if (text_widgets.at(i)->geometry().right() >= pos.x()) // 移到这个 label 的后面
                {
                    to_index = i;
                    break;
                }
            }

            // 获取被拖拽的目标
            int from_index = text_widgets.indexOf(label);
            if (from_index == -1) // 外面拖进来的
            {
                auto widget = new TimelineTextLabel(label, this);
                connectWidgetEvent(widget);
                widget->move(label->x(), label->getGlobalPos().y() - this->pos().y());
                widget->show();
                emit label->signalDraggedToOut(); // 从父类那里删掉

                text_widgets.insert(to_index, widget);
                adjustBucketSize(); // 从其他bucket那里移动过来，需要手动更换位置
            }
            else // 自己的，删掉旧的
            {
                auto widget = text_widgets.at(from_index);
                text_widgets.removeAt(from_index);
                text_widgets.insert(to_index, widget);
            }

            repaint();
            adjustWidgetsPositionsWithAnimation();
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

int TimelineBucket::getWaterProp()
{
    return water_prop;
}

void TimelineBucket::setWaterProp(int p)
{
    water_prop = p;
    update();
}
