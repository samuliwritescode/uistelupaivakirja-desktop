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
    if(!m_stats) return QString();

    switch(source)
    {
    case eStatistics:
        {
//            stats.setScaling(true);
            QList<QMap<QString, QString> > chartlist = m_stats->stats3D();
            QString retval;
            for(int loop=0; loop < chartlist.size(); loop++)
            {
                QMap<QString, QString> chart = chartlist.at(loop);
                retval += "**BEGIN**\n";
                for(QMap<QString, QString>::iterator iter = chart.begin(); iter != chart.end(); iter++)
                {
                    retval += iter.key();
                    retval += "\t";
                    retval += iter.value();
                    retval += "\n";
                }
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
    case eStatisticsColumn: m_stats->setX(value); break;
    case eStatisticsByColumn: m_stats->setZ(value); break;
    case eStatisticsField: m_stats->setUnitField(value); break;
    case eStatistics:
        if(m_stats->getName() == value)
        {
            break;
        }
        if(FishStatistics().getName() == value)
        {
            delete m_stats;
            m_stats = new FishStatistics();
        }
        else if(TripStatistics().getName() == value)
        {
            delete m_stats;
            m_stats = new TripStatistics();
        }

        sendNotificationToObservers(Controller::eStatisticsEngineUpdated);
        return;
        break;
    default: break;
    }
    sendNotificationToObservers(Controller::eStatisticsUpdated);
}

QMap<QString, QString> StatisticsController::getStats()
{
    if(m_stats)
        return m_stats->stats();

    return QMap<QString, QString>();
}

QList<QMap<QString, QString> > StatisticsController::getStats3D()
{
    if(m_stats)
        return m_stats->stats3D();

    return QList<QMap<QString, QString> >();
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

QStringList StatisticsController::getEngines()
{
    QStringList list;
    list << FishStatistics().getName();
    list << TripStatistics().getName();
    return list;
}
