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

void StatisticsController::textEvent(EUISource source, const QString& value)
{
    qDebug() << "got text event";
    switch(source)
    {
    case eStatisticsColumn: m_stats->setY(value); break;
    case eStatisticsField: m_stats->setUnitField(value); break;
    case eStatisticsUnit:
        if(value == "Kalojen määrä")
            m_stats->setUnit(TrollingStatistics::eCount);
        else if(value == "Kaloja tunnissa")
            m_stats->setUnit(TrollingStatistics::eFishPerTime);
        else if(value == "Keskiarvo")
            m_stats->setUnit(TrollingStatistics::eMean);
        else if(value == "Summa")
            m_stats->setUnit(TrollingStatistics::eSum);
        break;
    default: break;
    }
    sendNotificationToObservers(Controller::eStatisticsUpdated);
}
