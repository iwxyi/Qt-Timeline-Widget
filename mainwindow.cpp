#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand(time(0));

    (new TimelineUndoStack(ui->widget));
    ui->widget_2->setTimeline(ui->widget);

    for (int i = 0; i < 20; i++)
    {
        QStringList sl;
        int count = rand() % 8;
        while (count--)
            sl << ("文本"+QString::number(rand()%9999999900+100));
        ui->widget->addItem("2020.3."+QString::number(i), sl);
    }
    ui->widget->adjustBucketsPositionsWithAnimation();
}

MainWindow::~MainWindow()
{
    delete ui;
}

