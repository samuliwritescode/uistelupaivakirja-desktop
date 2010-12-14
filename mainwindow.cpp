#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "uistelu.h"
#include "ui_uistelu.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    qDebug() << "Clicked new";
    Uistelu uistelu;
    uistelu.show();
    uistelu.exec();
    qDebug() << "finished";
}
