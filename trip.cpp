#include "trip.h"
#include "ui_trip.h"

Trip::Trip(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Trip)
{
    ui->setupUi(this);
    ui->m_place->insertItem(0, "Ristinselkä");
    ui->m_place->insertItem(0, "Murtoselkä");
    ui->m_place->insertItem(0, "Vanhanselkä");

    for(int loop=0; loop < 50; loop++)
    {
        ui->scrollAreaWidgetContents->setLayout(ui->gridLayout);

        QLineEdit* hauki = new QLineEdit("hauki");


        ui->gridLayout->addWidget(hauki, loop, 0);
        ui->gridLayout->addWidget(new QLineEdit("6kg"), loop, 1);
        ui->gridLayout->addWidget(new QLineEdit("85cm"), loop, 2);
        ui->gridLayout->addWidget(new QLineEdit("hk varma"), loop, 3);
    }
}

Trip::~Trip()
{
    delete ui;
}
