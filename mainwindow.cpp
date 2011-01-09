#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trip.h"
#include "lures.h"


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


void MainWindow::on_newTripBtn_clicked()
{
    qDebug() << "Clicked new";
    Trip uistelu;
    uistelu.show();
    uistelu.exec();
    qDebug() << "finished";
}

void MainWindow::on_luresBtn_clicked()
{
    qDebug() << "Clicked new";
    Lures lures;
    lures.show();
    lures.exec();
    qDebug() << "finished";
}
