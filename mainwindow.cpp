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
        ui->listWidget->addItem("2020.3.13", sl);
    }
    ((QListWidget*)ui->listWidget)->addItem("aaaaaaaaaaa");

    ui->listWidget_2->hide();
    ui->listWidget_2->addItem("aaaaaaaaaaaaaa1111");
    ui->listWidget_2->addItem("aaaaaaaaaaaaaa2222");
    ui->listWidget_2->addItem("aaaaaaaaaaaaaa3333");
    ui->listWidget_2->addItem("aaaaaaaaaaaaaa4444");
    ui->listWidget_2->addItem("aaaaaaaaaaaaaa5555");
    ui->listWidget_2->addItem("aaaaaaaaaaaaaa6666");
}

MainWindow::~MainWindow()
{
    delete ui;
}

