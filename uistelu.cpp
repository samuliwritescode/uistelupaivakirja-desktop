#include "uistelu.h"
#include "ui_uistelu.h"

Uistelu::Uistelu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Uistelu)
{
    ui->setupUi(this);
    ui->m_startTime->insertItem(0, tr("aamu"));
    ui->m_startTime->insertItem(0, QString("keskipäivä"));
    ui->m_startTime->insertItem(0, "iltapäivä");
    ui->m_startTime->insertItem(0, "ilta");
    ui->m_startTime->insertItem(0, "yö");

}

Uistelu::~Uistelu()
{
    delete ui;
}
