#include "statisticscontroller.h"
#include "tripstatistics.h"
#include "fishstatistics.h"

StatisticsController::StatisticsController(QObject *parent) :
    Controller(parent),
    m_stats(NULL)
{
    m_stats = new FishStatistics();
    connect(m_stats, SIGNAL(progress(int)), this, SIGNAL(progress(int)));
}

QString StatisticsController::getTextValue(EUISource source)
{
    if(!m_stats) return QString();

    switch(source)
    {
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
            disconnect(m_stats, SIGNAL(progress(int)), this, SIGNAL(progress(int)));
            delete m_stats;
            m_stats = new FishStatistics();
            connect(m_stats, SIGNAL(progress(int)), this, SIGNAL(progress(int)));
        }
        else if(TripStatistics().getName() == value)
        {
            disconnect(m_stats, SIGNAL(progress(int)), this, SIGNAL(progress(int)));
            delete m_stats;
            m_stats = new TripStatistics();
            connect(m_stats, SIGNAL(progress(int)), this, SIGNAL(progress(int)));
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

TrollingStatisticsTable StatisticsController::getStats3D()
{
    if(m_stats)
        return m_stats->stats3D();

    return TrollingStatisticsTable();
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
