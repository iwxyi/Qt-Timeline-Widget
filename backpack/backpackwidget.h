#ifndef BACKPACKWIDGET_H
#define BACKPACKWIDGET_H

#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include "timelinewidget.h"

class BackpackWidget : public QWidget
{
    Q_OBJECT
public:
    BackpackWidget(QWidget *parent = nullptr);
    void setTimeline(TimelineWidget* timeline);

    struct TimeThing {
        QString name;   // 物品的名字
        QString value;  // 物品的值（只有可修改项才有）
        int time_index; // 对应时间轴的下标
    };

private:
    void initView();

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:

public slots:
    void autoRefreshTimeline();
    void refreshTimeline();
    void refreshThings();
    void slotComboChanged(const QString& name);

private:
    QComboBox* bps_combo;
    QListWidget* list_widget;
    QPushButton* refresh_btn;
    TimelineWidget* timeline;

    QHash<QString, QList<TimeThing>> backpacks;
    QString current_backpack;

    bool auto_refresh = true;
    bool is_focusing_item = false;
};

#endif // BACKPACKWIDGET_H
