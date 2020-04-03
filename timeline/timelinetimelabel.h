#ifndef TIMELINETIMELABEL_H
#define TIMELINETIMELABEL_H

#include <QObject>
#include <QApplication>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QDate>
#include <QMouseEvent>

class TimelineTimeLabel : public QLabel
{
    Q_OBJECT
public:
    TimelineTimeLabel(QWidget* parent = nullptr);

    void adjustSize(bool notify = true);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void signalClicked();
    void signalDoubleClicked();
    void signalSizeChanged(QSize size);

public slots:
    void slotMenuShowed(const QPoint &pos);

private:
    QPoint press_pos;
};

#endif // TIMELINETIMELABEL_H
