#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->listWidget->addItem("2020.3.13", "这是文本");
    ui->listWidget->addItem("2020.3.13", QStringList{"这是文本1", "这是文本2", "这是文本3", "这是文本4"});
}

MainWindow::~MainWindow()
{
    delete ui;
}

