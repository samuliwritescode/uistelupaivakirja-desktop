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

bool StatisticsController::getBooleanValue(EUISource source)
{
    if(source == eStatisticsUnit)
    {
        return m_stats->supportOperand();
    }
    return false;
}

void StatisticsController::textEvent(EUISource source, const QString& value)
{
    switch(source)
    {
    case eStatisticsColumn: m_stats->setX(value); break;
    case eStatisticsByColumn: m_stats->setZ(value); break;
    case eStatisticsField: m_stats->setOperand(value); break;
    case eStatisticsUnit: m_stats->setOperator(value); break;
    case eStatisticsFilterClear: m_stats->clearFilter(); break;
    case eStatisticsFilterField:
        m_stats->clearFilter();
        m_filterField = value; break;
    case eStatisticsFilterComparison: m_stats->setFilterComparison(m_filterField, value); break;
    case eStatisticsFilterText: m_stats->setFilterText(m_filterField, value); break;
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

QHash<QString, QString> StatisticsController::getStats()
{
    if(m_stats)
        return m_stats->stats();

    return QHash<QString, QString>();
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
    list.sort();
    return list;
}

QStringList StatisticsController::getNumericFields()
{
    QStringList list = m_stats->getNumericFields();
    list.sort();
    return list;
}

QStringList StatisticsController::getEngines()
{
    QStringList list;
    list << FishStatistics().getName();
    list << TripStatistics().getName();
    return list;
}

QStringList StatisticsController::getOperators()
{
    QStringList list = m_stats->getOperators();
    return list;
}

QStringList StatisticsController::getComparisonOperators()
{
    QStringList retval;

    if(m_stats->getNumericFields().contains(m_filterField))
    {
        retval = m_stats->getNumericComparisonOperators();
    }
    else if(m_stats->getTextFields().contains(m_filterField))
    {
        retval = m_stats->getTextComparisonOperators();
    }

    retval.sort();
    return retval;
}
