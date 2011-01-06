#include "trip.h"
#include "ui_trip.h"
#include "lures.h"
#include "lureitem.h"

Trip::Trip(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Trip)
{
    ui->setupUi(this);
    ui->m_place->insertItem(0, "Ristinselkä");
    ui->m_place->insertItem(0, "Murtoselkä");
    ui->m_place->insertItem(0, "Vanhanselkä");
    ui->scrollAreaWidgetContents->setLayout(ui->gridLayout);

    for(int loop=0; loop < 10; loop++)
    {
        LureItem* draggable = new LureItem("HK varma");

        QComboBox* kalat = new QComboBox();
        kalat->insertItem(0, "hauki");
        kalat->insertItem(0, "ahven");
        draggable->setAcceptDrops(true);
        ui->gridLayout->addWidget(kalat, loop, 0);
        ui->gridLayout->addWidget(new QLineEdit("6kg"), loop, 1);
        ui->gridLayout->addWidget(new QLineEdit("85cm"), loop, 2);
        ui->gridLayout->addWidget(draggable, loop, 3);
    }

    Lures* lures = new Lures(this);
    lures->show();
}

Trip::~Trip()
{
    delete ui;
}
