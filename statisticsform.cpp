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
        TrollingStatisticsTable stats = m_statsController->getStats3D();
        m_statWidget->setCols(stats.m_columns);
        m_statWidget->clearStat();
        for(int loop=0; loop < stats.m_data.count(); loop++)
        {
            m_statWidget->addStat(stats.m_data.at(loop), stats.m_rows.at(loop));
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

        setupCombo(ui->byColumnCombo, m_statsController->getFields());
        setupCombo(ui->columnCombo, m_statsController->getFields());
        setupCombo(ui->calculatefromfieldCombo, m_statsController->getNumericFields());
        setupCombo(ui->unitCombo, m_statsController->getOperators());
        ui->unitCombo->setCurrentIndex(0);
        setupCombo(ui->filterCombo, m_statsController->getFields());

        ui->lineEditFilter->setText("");
    }
}

void StatisticsForm::setupCombo(QComboBox* p_combo, const QStringList& p_content)
{
    p_combo->blockSignals(true);
    p_combo->clear();
    for(int loop=0; loop < p_content.count(); loop++)
    {
        p_combo->addItem(p_content.at(loop));
    }
    p_combo->setCurrentIndex(-1);
    p_combo->blockSignals(false);
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
    if(checked)
    {
       m_statsController->textEvent(eFishList, "");
    }
    else //reset Z when 3D is not on.
    {
        m_statsController->textEvent(eStatisticsByColumn, "");
        ui->byColumnCombo->setCurrentIndex(-1);
    }
}

void StatisticsForm::on_filterCombo_currentIndexChanged(QString text)
{
   m_statsController->textEvent(eStatisticsFilterField, text);
   setupCombo(ui->comparisonCombo, m_statsController->getComparisonOperators());
   ui->lineEditFilter->clear();
}

void StatisticsForm::on_comparisonCombo_currentIndexChanged(QString text)
{
    m_statsController->textEvent(eStatisticsFilterComparison, text);
}

void StatisticsForm::on_lineEditFilter_textEdited(QString text)
{
    m_statsController->textEvent(eStatisticsFilterText, text);
}

void StatisticsForm::on_checkBoxFilter_clicked(bool checked)
{
    if(!checked)
    {
        m_statsController->textEvent(eStatisticsFilterClear, "");
        ui->filterCombo->setCurrentIndex(-1);
        ui->comparisonCombo->setCurrentIndex(-1);
        ui->lineEditFilter->clear();
    }

    ui->filterCombo->setDisabled(!checked);
    ui->lineEditFilter->setDisabled(!checked);
    ui->comparisonCombo->setDisabled(!checked);
}
