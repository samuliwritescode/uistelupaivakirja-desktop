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
            stats.setY(tr("Paino"));
            stats.setScaling(true);
            stats.setUnit(TrollingStatistics::eCount);
            stats.setUnitField(tr("Paino"));
            QMap<QString, QString> chart = stats.stats();
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
