#include "statisticscontroller.h"
#include "trollingstatistics.h"

StatisticsController::StatisticsController(QObject *parent) :
    Controller(parent)
{
}

QString StatisticsController::getTextValue(EUISource source)
{
    switch(source)
    {
    case eStatistics:
        {
            TrollingStatistics stats;
            return stats.stats();
        }
        break;
    default: break;
    }
    return QString();
}
