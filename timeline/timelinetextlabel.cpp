#include "timelinetextlabel.h"

TimelineTextLabel::TimelineTextLabel(QWidget *parent) : QLabel(parent)
{
    setObjectName("TimelineTextLabel");

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested (const QPoint&)),this,SLOT(slotMenuShowed(const QPoint&)));
    setWordWrap(true); // 根据宽度自动换行（ 但是需要手动确定宽高）
}

TimelineTextLabel::TimelineTextLabel(TimelineTextLabel *&another, QWidget *parent) : TimelineTextLabel(parent)
{
    setText(another->text());
    resize(another->size());
}

void TimelineTextLabel::adjustSize(bool notify, QString text)
{
    QSize old_size = size();
//    if (text.isEmpty())
//        text = this->text();
    {
        QFontMetrics fm(this->font());
        int line_height = fm.lineSpacing();
        int total_width = fm.horizontalAdvance(text);
        if (total_width <= 20)
        {
            setMinimumWidth(0);
//            QLabel::adjustSize();
        }
        else
        {
            // 选择合适的比例
            int line_count =static_cast<int>(sqrt(qMax(total_width / line_height / 5, 1)));
            int text_width = (total_width + line_count - 1) / line_count;
//            QRect rect = fm.boundingRect(QRect(0,0,text_width,0), Qt::TextWordWrap, text);
//            qDebug() << line_count << text_width << rect.height();
            setMinimumWidth(text_width + 20); // padding=10
//            resize(text_width+20, height());
//            resize(rect.width(), rect.height()+20);
        }
        QLabel::adjustSize();
    }

    if (notify && old_size != size())
        emit signalSizeChanged(size());
}

QPoint TimelineTextLabel::getGlobalPos()
{
    return pos() + parentWidget()->pos();
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
            mime->setText(this->text());
            QDrag* drag = new QDrag(this);
            drag->setMimeData(mime);
            drag->setHotSpot(event->pos()); // 好像没什么用
            QPixmap pixmap(size());
            pixmap.fill(Qt::transparent);
            render(&pixmap);
            {
                // 保存临时文件
                QString path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
                QString text = this->text();
                text.replace("\\", "").replace("/", "").replace(":", "").replace("*", "").replace("?", "").replace("\"", "")
                        .replace("<", "").replace(">", "").replace("|", "");
                if (text.isEmpty())
                    text = "未定";
                else if (text.length() > 8)
                    text = text.left(8);
                path += "/"+text+".png";
                pixmap.save(path);
                mime->setUrls(QList<QUrl>{QUrl("file:///" + path)});
            }
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
    QAction* insert_left_action = new QAction("左边插入文字节点", this);
    QAction* insert_right_action = new QAction("右边插入文字节点", this);
    QAction* delete_action = new QAction("删除节点", this);
    QAction* copy_text_action = new QAction("复制节点文字", this);
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
