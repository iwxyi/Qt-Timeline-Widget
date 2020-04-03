#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand(time(0));

    for (int i = 0; i < 20; i++)
    {
        QStringList sl;
        int count = rand() % 8;
        while (count--)
            sl << ("文本"+QString::number(rand()%90000+10000));
        ui->widget->addItem("2020.3."+QString::number(i), sl/*QStringList{QString::number(i)}*/);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

