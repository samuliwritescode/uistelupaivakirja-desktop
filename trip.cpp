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
    ui->scrollAreaWidgetContents->setLayout(ui->gridLayout);

    for(int loop=0; loop < 10; loop++)
    {
        QLabel* draggable = new QLabel("keijjo");

        QComboBox* kalat = new QComboBox();
        kalat->insertItem(0, "hauki");
        kalat->insertItem(0, "ahven");
        draggable->setAcceptDrops(true);
        ui->gridLayout->addWidget(kalat, loop, 0);
        ui->gridLayout->addWidget(new QLineEdit("6kg"), loop, 1);
        ui->gridLayout->addWidget(new QLineEdit("85cm"), loop, 2);
        ui->gridLayout->addWidget(new QLineEdit("hk varma"), loop, 3);
        ui->gridLayout->addWidget(draggable, loop, 4);
    }
}

Trip::~Trip()
{
    delete ui;
}
