#include "statisticsform.h"
#include "ui_statisticsform.h"
#include "glstatwidget.h"

StatisticsForm::StatisticsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsForm)
{
    ui->setupUi(this);
    m_statWidget = new GLStatWidget();
    ui->statisticslayout->addWidget(m_statWidget);

    m_statsController = Singletons::statsController();
    connect(m_statsController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    connect(m_statsController, SIGNAL(progress(int)), this, SLOT(progressBarChanged(int)));

    observerEvent(Controller::eStatisticsEngineUpdated);
    QStringList engines = m_statsController->getEngines();
    for(int loop=0; loop < engines.count(); loop++)
    {
        ui->statisticsCombo->addItem(engines.at(loop));
    }
    ui->byColumnCombo->setVisible(false);

}

StatisticsForm::~StatisticsForm()
{
    delete ui;
}

void StatisticsForm::observerEvent(int type)
{
    if(type == Controller::eStatisticsUpdated)
    {
        if(ui->checkBox3D->isChecked())
        {
            TrollingStatisticsTable stats = m_statsController->getStats3D();
            m_statWidget->setCols(stats.m_columns);
            m_statWidget->clearStat();
            for(int loop=0; loop < stats.m_data.count(); loop++)
            {
                m_statWidget->addStat(stats.m_data.at(loop), stats.m_rows.at(loop));
            }
        }
        else
        {
            QHash<QString, QString> stats = m_statsController->getStats();
            QList<QString> cols = stats.keys();
            qSort(cols);
            m_statWidget->setCols(cols);
            m_statWidget->clearStat();
            m_statWidget->addStat(stats, "");
        }

        if(m_statsController->getBooleanValue(eStatisticsUnit))
        {
            ui->calculatefromfieldCombo->show();
            ui->labelFromField->show();
        }
        else
        {
            ui->calculatefromfieldCombo->hide();
            ui->labelFromField->hide();
        }
    }
    else if(type == Controller::eStatisticsEngineUpdated)
    {
        qDebug() << "update stats engine";
        ui->columnCombo->blockSignals(true);
        ui->columnCombo->clear();
        ui->byColumnCombo->blockSignals(true);
        ui->byColumnCombo->clear();
        QStringList fields = m_statsController->getFields();
        for(int loop=0; loop < fields.count(); loop++)
        {
            ui->columnCombo->addItem(fields.at(loop));
            ui->byColumnCombo->addItem(fields.at(loop));
        }
        ui->columnCombo->setCurrentIndex(-1);
        ui->byColumnCombo->setCurrentIndex(-1);

        ui->byColumnCombo->blockSignals(false);
        ui->columnCombo->blockSignals(false);

        ui->calculatefromfieldCombo->blockSignals(true);
        ui->calculatefromfieldCombo->clear();
        QStringList numFields = m_statsController->getNumericFields();
        for(int loop=0; loop < numFields.count(); loop++)
        {
            ui->calculatefromfieldCombo->addItem(numFields.at(loop));
        }
        ui->calculatefromfieldCombo->setCurrentIndex(-1);
        ui->calculatefromfieldCombo->blockSignals(false);

        ui->unitCombo->blockSignals(true);
        ui->unitCombo->clear();
        QStringList operators = m_statsController->getOperators();
        for(int loop=0; loop < operators.count(); loop++)
        {
            ui->unitCombo->addItem(operators.at(loop));
        }
        ui->unitCombo->blockSignals(false);
    }
}

void StatisticsForm::progressBarChanged(int value)
{
    m_statWidget->setProgress(value);
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

void StatisticsForm::on_unitCombo_currentIndexChanged(int index)
{
}

void StatisticsForm::on_statisticsCombo_currentIndexChanged(QString value)
{
    m_statsController->textEvent(eStatistics, value);
}

void StatisticsForm::on_byColumnCombo_currentIndexChanged(QString value)
{
    m_statsController->textEvent(eStatisticsByColumn, value);
}

void StatisticsForm::on_checkBox3D_clicked(bool checked)
{
    ui->byColumnCombo->setVisible(checked);
    //send something to controller to get update.
    m_statsController->textEvent(eFishList, "");
}
