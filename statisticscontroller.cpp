#include "statisticscontroller.h"
#include "tripstatistics.h"
#include "fishstatistics.h"

StatisticsController::StatisticsController(QObject *parent) :
    Controller(parent),
    m_stats(NULL)
{
    m_stats = new FishStatistics();
}

QString StatisticsController::getTextValue(EUISource source)
{
    switch(source)
    {
    case eStatistics:
        {
//            stats.setScaling(true);
            QMap<QString, QString> chart = m_stats->stats();
            QString retval;
            for(QMap<QString, QString>::iterator iter = chart.begin(); iter != chart.end(); iter++)
            {
                retval += iter.key();
                retval += "\t";
                retval += iter.value();
                retval += "\n";
            }
            return retval;
        }
        break;
    default: break;
    }
    return QString();
}

void StatisticsController::intEvent(EUISource source, int value)
{
    switch(source)
    {
    case eStatisticsUnit: m_stats->setUnit(static_cast<TrollingStatistics::EUnit>(value)); break;
    default: break;
    }
    sendNotificationToObservers(Controller::eStatisticsUpdated);
}

void StatisticsController::textEvent(EUISource source, const QString& value)
{
    switch(source)
    {
    case eStatisticsColumn: m_stats->setY(value); break;
    case eStatisticsField: m_stats->setUnitField(value); break;
    default: break;
    }
    sendNotificationToObservers(Controller::eStatisticsUpdated);
}

QStringList StatisticsController::getFields()
{
    QStringList list = m_stats->getTextFields();
    list.append(m_stats->getNumericFields());
    return list;
}

QStringList StatisticsController::getNumericFields()
{
    return m_stats->getNumericFields();
}
