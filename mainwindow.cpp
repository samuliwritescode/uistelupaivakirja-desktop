#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lureitem.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_controller = new TrollingController();

    ui->place->insertItem(0, "Ristinselkä");
    ui->place->insertItem(0, "Murtoselkä");
    ui->place->insertItem(0, "Vanhanselkä");

    ui->specie->insertItem(0, "Hauki");
    ui->specie->insertItem(0, "Ahven");
    ui->specie->insertItem(0, "Kuha");


    for(int loop=0; loop < 10; loop++)
    {
        LureItem* draggable = new LureItem("HK varma");
        LureItem* poi = new LureItem("12:22 hauki 13.3-1");

        QComboBox* kalat = new QComboBox();
        kalat->insertItem(0, "hauki");
        kalat->insertItem(0, "ahven");
        draggable->setAcceptDrops(true);
      /*  ui->catchGrid->addWidget(kalat, loop, 0);
        ui->catchGrid->addWidget(new QLineEdit("6kg"), loop, 1);
        ui->catchGrid->addWidget(new QLineEdit("85cm"), loop, 2);
        ui->catchGrid->addWidget(new QLineEdit("22C"), loop, 3);
        ui->catchGrid->addWidget(new QLineEdit("5m"), loop, 4);
        ui->catchGrid->addWidget(draggable, loop, 5);
        ui->catchGrid->addWidget(poi, loop, 6);*/
    }

    //ui->lureList->setIconSize(QSize(100,50));
    for(int loop=0; loop < 100; loop++)
    {
        QListWidgetItem* item = new QListWidgetItem("HK varma taimen 9 cm venäjän lippu"+QString::number(loop));
        item->setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        item->setIcon(QIcon(":/msnlogo.jpg"));
        ui->lureList->addItem(item);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_controller;
}

