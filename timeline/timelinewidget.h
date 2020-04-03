#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QObject>
#include <QDebug>
#include <QInputDialog>
#include <QMenu>
#include <QAction>
#include <QMimeData>
#include <QDrag>
#include <QPropertyAnimation>
#include <QScrollArea>
#include <QScrollBar>
#include <QPlainTextEdit>
#include <QTimer>
#include "timelinebucket.h"
#include "labeleditor.h"

class TimelineWidget : public QScrollArea
{
    Q_OBJECT
public:
    TimelineWidget(QWidget *parent = nullptr);

    void addItem(QString time, QString text);
    void addItem(QString time, QStringList texts);
    TimelineBucket* insertItem(QString time, QStringList texts, int index = -1);
    void removeItem(int index);
    void clearAll();
    int count();

    void selectAll();
    void unselectAll();
    void selectItem(TimelineBucket* bucket);
    void unselectItem(TimelineBucket* bucket);
    void setCurrentItem(int row, bool multi = false);
    void setCurrentItem(TimelineBucket* bucket, bool multi = false);
    void scrollTo(int index = -1);

    void adjustBucketsPositions(int start = -1);
    void adjustBucketsPositionsWithAnimation(int start = 0, int end = -1);

    void fromString(QString string, QString time_reg = "【%1】", QString para_split = "\n", QString line_split = "\n\n");
    QString toString(QString time_format = "【%1】", QString para_split = "\n", QString line_split = "\n\n");

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    TimelineBucket *createItemWidget(QString time, QStringList texts);

signals:

public slots:
    void updateUI();
    void slotBucketWidgetToSelect(TimelineBucket* bucket);
    void slotTimeWidgetClicked(TimelineTimeLabel* label);
    void slotTextWidgetClicked(TimelineTextLabel* label);
    void slotTimeWidgetDoubleClicked(TimelineTimeLabel* label);
    void slotTextWidgetDoubleClicked(TimelineTextLabel* label);
    void slotMenuShowed(const QPoint& pos);
    void slotDroppedAndMoved(TimelineBucket* from, TimelineBucket* to);
    void slotEditChanged();
    void hideEditing();

    void actionAddText();
    void actionAddLine();
    void actionInsertAbove();
    void actionInsertUnder();
    void actionDeleteLine();
    void actionCopyText();

private:
    QWidget* center_widget;
    QList<TimelineBucket*> buckets;
    QList<TimelineBucket*> selected_buckets;
    int current_index;

    LabelEditor* edit;
    TimelineBucket* editing_bucket;
    QLabel* editing_label;
};

#endif // TIMELINEWIDGET_H
