#include "lures.h"
#include "ui_lures.h"

Lures::Lures(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Lures)
{
    ui->setupUi(this);
/*    ui->scrollAreaWidgetContents->setLayout(ui->luretable);

    for(int loop=0; loop < 50; loop++)
    {
        ui->luretable->addWidget(new QLineEdit("HK varma"), loop, 0);
        ui->luretable->addWidget(new QLineEdit("neulamuikku"), loop, 1);
        ui->luretable->addWidget(new QLineEdit("9cm"), loop, 2);
        ui->luretable->addWidget(new QLineEdit("sinipuna"), loop, 3);
    }*/
}

Lures::~Lures()
{
    delete ui;
}
