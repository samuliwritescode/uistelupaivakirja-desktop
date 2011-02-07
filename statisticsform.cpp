#include "statisticsform.h"
#include "ui_statisticsform.h"

StatisticsForm::StatisticsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsForm)
{
    ui->setupUi(this);
    m_statsController = Singletons::statsController();
    connect(m_statsController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));

    observerEvent(Controller::eStatisticsEngineUpdated);
    QStringList engines = m_statsController->getEngines();
    for(int loop=0; loop < engines.count(); loop++)
    {
        ui->statisticsCombo->addItem(engines.at(loop));
    }

    ui->unitCombo->addItem(tr("Määrä"));
    ui->unitCombo->addItem(tr("Riviä tunnissa"));
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
    else if(type == Controller::eStatisticsEngineUpdated)
    {
        qDebug() << "update stats engine";
        ui->columnCombo->blockSignals(true);
        ui->columnCombo->clear();
        QStringList fields = m_statsController->getFields();
        for(int loop=0; loop < fields.count(); loop++)
        {
            ui->columnCombo->addItem(fields.at(loop));
        }
        ui->columnCombo->blockSignals(false);

        ui->calculatefromfieldCombo->blockSignals(true);
        ui->calculatefromfieldCombo->clear();
        QStringList numFields = m_statsController->getNumericFields();
        for(int loop=0; loop < numFields.count(); loop++)
        {
            ui->calculatefromfieldCombo->addItem(numFields.at(loop));
        }
        ui->calculatefromfieldCombo->blockSignals(false);
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

void StatisticsForm::on_statisticsCombo_currentIndexChanged(QString value)
{
    m_statsController->textEvent(eStatistics, value);
}
