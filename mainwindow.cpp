#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statsTab->layout()->addWidget(new StatisticsForm());
    ui->tripsTab->layout()->addWidget(new TripForm());
    ui->luresTab->layout()->addWidget(new LureForm());
    ui->placesTab->layout()->addWidget(new PlaceForm());
}

MainWindow::~MainWindow()
{
    delete ui;
}


