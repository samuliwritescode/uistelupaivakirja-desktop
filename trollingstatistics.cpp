#include <QDebug>
#include "singletons.h"
#include "trollingstatistics.h"

TrollingStatistics::TrollingStatistics()
{
}

QString TrollingStatistics::stats()
{
    QString retval;
    QList<QMap<QString, QString> > statistics;
    QMap<int, Trip*> trips = Singletons::model()->getTrips();
    foreach(Trip* trip, trips)
    {
        //qDebug() << trip->getPlace()->getName();
        for(int loop=0; loop < trip->getFishCount(); loop++)
        {
            QMap<QString, QString> statline;
            Fish* fish = trip->getFish(loop);
            if(fish->getType() == Fish::eFish)
            {
                statline[tr("Laji")] = fish->getProperty(FISH_SPECIES).toString();
            }
            else if(fish->getType() == Fish::eWeather)
            {
                statline[tr("Sää")] = fish->getProperty(FISH_WEATHER).toInt();
            }
            else if(fish->getType() == Fish::eFishAndWeather)
            {
                statline[tr("Laji")] = fish->getProperty(FISH_SPECIES).toString();
            }
            else
            {

            }
            statistics.push_back(statline);
        }
    }

    QMap<QString, int> fishcount;
    for(int loop=0; loop < statistics.count(); loop++)
    {
        QMap<QString, QString> statline = statistics.at(loop);
        fishcount[statline[tr("Laji")]]++;
    }

    for(QMap<QString, int>::iterator iter= fishcount.begin(); iter!=fishcount.end(); iter++)
    {
        retval += iter.key();
        retval += "\t";
        retval += QString::number(iter.value());
        retval += "\n";
    }
    return retval;
}
