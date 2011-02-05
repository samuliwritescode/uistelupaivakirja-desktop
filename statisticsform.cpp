#include "statisticsform.h"
#include "ui_statisticsform.h"

StatisticsForm::StatisticsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsForm)
{
    ui->setupUi(this);
    m_statsController = Singletons::statsController();

    ui->statistics->setText(m_statsController->getTextValue(eStatistics));
}

StatisticsForm::~StatisticsForm()
{
    delete ui;
}
