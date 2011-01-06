#include "lures.h"
#include "ui_lures.h"

Lures::Lures(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Lures)
{
    ui->setupUi(this);
}

Lures::~Lures()
{
    delete ui;
}
