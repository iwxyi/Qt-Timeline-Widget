#ifndef TIMELINETEXTLABEL_H
#define TIMELINETEXTLABEL_H

#include <QObject>
#include <QApplication>
#include <QLabel>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QClipboard>
#include <QMouseEvent>
#include <QDebug>
#include <QMimeData>
#include <QDrag>
#include <cmath>
#include <QStandardPaths>
#include "timelineglobal.h"

#define TIMELINE_TEXT_MIME_KEY "TIMELINE_TEXT_WIDGET"

class TimelineBucket;

class TimelineTextLabel : public QLabel
{
    Q_OBJECT
public:
    TimelineTextLabel(QWidget* parent = nullptr);
    TimelineTextLabel(const TimelineTextLabel*& another, QWidget* parent = nullptr);

    void adjustSize(bool notify = true, QString text = "");

    QPoint getGlobalPos() const;
    void draggedToOut();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent *ev) override;

signals:
    void signalClicked();
    void signalDoubleClicked();
    void signalInsertLeft();
    void signalInsertRight();
    void signalDelete();
    void signalCopyText();
    void signalDraggedToOut();
    void signalSizeChanged(QSize size);

public slots:
    void slotMenuShowed(const QPoint &);

private:
    QPoint press_pos;

};

#endif // TIMELINETEXTLABEL_H
