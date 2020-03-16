#ifndef TIMELINELEADINGDOT_H
#define TIMELINELEADINGDOT_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>

class TimelineLeadingDot : public QWidget
{
    Q_OBJECT
public:
    TimelineLeadingDot(QWidget *parent = nullptr);

    void setColor(QColor c);
    void setRadius(int r);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:

public slots:

private:
    QColor bg_color;
};

#endif // TIMELINELEADINGDOT_H
