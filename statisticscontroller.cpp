#include "statisticscontroller.h"
#include "tripstatistics.h"

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
            TripStatistics stats;
            stats.setY(tr("Vuosi ja kk"));
            //stats.setScaling(true);
            stats.setUnit(TrollingStatistics::eMean);
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
