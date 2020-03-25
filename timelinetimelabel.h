#ifndef TIMELINETIMELABEL_H
#define TIMELINETIMELABEL_H

#include <QObject>
#include <QApplication>
#include <QLabel>
#include <QMouseEvent>

class TimelineTimeLabel : public QLabel
{
    Q_OBJECT
public:
    TimelineTimeLabel(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

signals:
    void signalClicked();

private:
    QPoint press_pos;
};

#endif // TIMELINETIMELABEL_H
