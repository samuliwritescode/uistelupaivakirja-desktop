#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lureitem.h"
#include "singletons.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());
    ui->place->insertItems(0, Singletons::placeController()->getPlaces());

    ui->species->insertItem(0, "Hauki");
    ui->species->insertItem(0, "Ahven");
    ui->species->insertItem(0, "Kuha");


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
}

void MainWindow::on_dateEdit_dateChanged(QDate date)
{
    Singletons::tripController()->setDate(ui->dateEdit->date());
}

void MainWindow::on_time4_9_clicked(bool checked)
{
    checked?Singletons::tripController()->addTime(4, 9):Singletons::tripController()->removeTime(4,9);
}

void MainWindow::on_time9_11_clicked(bool checked)
{
    checked?Singletons::tripController()->addTime(9, 11):Singletons::tripController()->removeTime(4,9);
}

void MainWindow::on_time11_14_clicked(bool checked)
{
    checked?Singletons::tripController()->addTime(11, 14):Singletons::tripController()->removeTime(4,9);
}

void MainWindow::on_time14_18_clicked(bool checked)
{
    checked?Singletons::tripController()->addTime(14, 18):Singletons::tripController()->removeTime(4,9);
}

void MainWindow::on_time18_23_clicked(bool checked)
{
    checked?Singletons::tripController()->addTime(18, 23):Singletons::tripController()->removeTime(4,9);
}

void MainWindow::on_time23_4_clicked(bool checked)
{
    checked?Singletons::tripController()->addTime(23, 4):Singletons::tripController()->removeTime(4,9);
}

void MainWindow::on_place_currentIndexChanged(QString place)
{
    Singletons::tripController()->setPlace(place);
}

void MainWindow::on_trip_save_clicked()
{
    Singletons::tripController()->saveTrip();
}

void MainWindow::on_water_temp_textChanged(QString temp)
{
    Singletons::tripController()->setWaterTemp(temp);
}
