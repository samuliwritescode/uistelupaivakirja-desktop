#include "statisticscontroller.h"
#include "tripstatistics.h"
#include "fishstatistics.h"

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
            FishStatistics stats;
            stats.setY(tr("Tuulen suunta"));
            //stats.setScaling(true);
            stats.setUnit(TrollingStatistics::eCount);
            stats.setUnitField(tr("Reissun pituus"));
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
