#include "statisticsform.h"
#include "ui_statisticsform.h"

StatisticsForm::StatisticsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsForm)
{
    ui->setupUi(this);
    m_statsController = Singletons::statsController();
    connect(m_statsController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));

    QStringList fields = m_statsController->getFields();
    for(int loop=0; loop < fields.count(); loop++)
    {
        ui->columnCombo->addItem(fields.at(loop));
    }

    QStringList numFields = m_statsController->getNumericFields();
    for(int loop=0; loop < numFields.count(); loop++)
    {
        ui->calculatefromfieldCombo->addItem(numFields.at(loop));
    }

    ui->unitCombo->addItem(tr("Kalojen määrä"));
    ui->unitCombo->addItem(tr("Kaloja tunnissa"));
    ui->unitCombo->addItem(tr("Keskiarvo"));
    ui->unitCombo->addItem(tr("Summa"));

}

StatisticsForm::~StatisticsForm()
{
    delete ui;
}

void StatisticsForm::observerEvent(int type)
{
    if(type == Controller::eStatisticsUpdated)
    {
        qDebug() << "update stats";
        ui->statistics->setText(m_statsController->getTextValue(eStatistics));
    }
}

void StatisticsForm::on_unitCombo_textChanged(QString text)
{

}

void StatisticsForm::on_calculatefromfieldCombo_textChanged(QString text)
{

}

void StatisticsForm::on_columnCombo_textChanged(QString text)
{

}

void StatisticsForm::on_unitCombo_currentIndexChanged(QString text)
{

}

void StatisticsForm::on_calculatefromfieldCombo_currentIndexChanged(QString text)
{
    m_statsController->textEvent(eStatisticsField, text);
}

void StatisticsForm::on_columnCombo_currentIndexChanged(QString text)
{
    m_statsController->textEvent(eStatisticsColumn, text);
}

void StatisticsForm::on_unitCombo_currentIndexChanged(int index)
{
    m_statsController->intEvent(eStatisticsUnit, index);
}
