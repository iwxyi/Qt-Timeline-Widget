#ifndef TESTLISTWIDGET_H
#define TESTLISTWIDGET_H

#include <QObject>
#include <QListWidget>
#include <QDragEnterEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QDebug>

class TestListWidget : public QListWidget
{
public:
    TestListWidget(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;

private:
    QPoint press_pos;
};

#endif // TESTLISTWIDGET_H
