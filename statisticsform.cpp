#include "statisticsform.h"
#include "ui_statisticsform.h"

StatisticsForm::StatisticsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsForm)
{
    ui->setupUi(this);
    m_statsController = Singletons::statsController();

    //ui->statistics->setText(m_statsController->getTextValue(eStatistics));
    connect(m_statsController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));

    ui->unitCombo->addItem("Kalojen määrä");
    ui->unitCombo->addItem("Kaloja tunnissa");
    ui->unitCombo->addItem("Keskiarvo");
    ui->unitCombo->addItem("Summa");

    ui->columnCombo->addItem("Laji");
    ui->columnCombo->addItem("Kalapaikka");
    ui->columnCombo->addItem("Vuosi ja kk");
    ui->columnCombo->addItem("Reissun pituus");
    ui->columnCombo->addItem("Säätila");
    ui->columnCombo->addItem("Lämpötila");

    ui->calculatefromfieldCombo->addItem("Reissun pituus");
    ui->calculatefromfieldCombo->addItem("Lämpötila");
    ui->calculatefromfieldCombo->addItem("Paino");
    ui->calculatefromfieldCombo->addItem("Pituus");
    ui->calculatefromfieldCombo->addItem("Kuukausi");
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
    m_statsController->textEvent(eStatisticsUnit, text);
}

void StatisticsForm::on_calculatefromfieldCombo_currentIndexChanged(QString text)
{
    m_statsController->textEvent(eStatisticsField, text);
}

void StatisticsForm::on_columnCombo_currentIndexChanged(QString text)
{
    m_statsController->textEvent(eStatisticsColumn, text);
}
