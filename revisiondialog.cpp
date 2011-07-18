#include <QDebug>
#include "revisiondialog.h"
#include "ui_revisiondialog.h"

RevisionDialog::RevisionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RevisionDialog)
{
    ui->setupUi(this);
    m_dontSave = false;
    QStringList headers;
    headers << tr("muuttunut kohde");
    ui->changelist->setHorizontalHeaderLabels(headers);
}

RevisionDialog::~RevisionDialog()
{
    delete ui;
}

void RevisionDialog::setModified(QStringList list)
{
    setItems(list, QColor::fromRgb(255,255,255));
}

void RevisionDialog::setConflicting(QStringList list)
{
    setItems(list, QColor::fromRgb(255,127,0));
}

void RevisionDialog::setAdded(QStringList list)
{
    setItems(list, QColor::fromRgb(0,255,0));
}

void RevisionDialog::setRemoved(QStringList list)
{
    setItems(list, QColor::fromRgb(255,0,0));
}

void RevisionDialog::setItems(QStringList list, QColor color)
{
    for(int loop=0; loop < list.size(); loop++)
    {
        int itemsnow = ui->changelist->rowCount();
        ui->changelist->insertRow(itemsnow);
        //QTableWidgetItem* item1 = new QTableWidgetItem("X", itemsnow);
        QTableWidgetItem* item2 = new QTableWidgetItem(list.at(loop), itemsnow);
        //item1->setBackgroundColor(color);
        item2->setBackgroundColor(color);
        //ui->changelist->setItem(itemsnow, 0, item1);
        ui->changelist->setItem(itemsnow, 0, item2);
    }
}

void RevisionDialog::on_buttonBox_clicked(QAbstractButton* button)
{
    QDialogButtonBox::StandardButton buttontype = ui->buttonBox->standardButton(button);
    if(buttontype == QDialogButtonBox::Discard)
    {
        m_dontSave = true;
        accept();
    }
}

bool RevisionDialog::isDontSave()
{
    return m_dontSave;
}
