#include "tripstatistics.h"

TripStatistics::TripStatistics(QObject *) :
    TrollingStatistics()
{
}


QMap<QString, QString> TripStatistics::stats()
{


    QList<QMap<QString, QString> > statistics;
    QMap<int, Trip*> trips = Singletons::model()->getTrips();
    foreach(Trip* trip, trips)
    {
        int kaloja = 0;
        for(int loop=0; loop < trip->getFishCount(); loop++)
        {
            Fish* fish = trip->getFish(loop);
            if(fish->getType() == Fish::eFish ||
               fish->getType() == Fish::eFishAndWeather)
            {
                kaloja++;
            }
        }

        QMap<QString, QString> statline;
        statline[tr("Kaloja")] = QString::number(kaloja);
        statline[tr("Kalapaikka")] = trip->getPlace()->getName();
        statline[tr("Päiväys")] = trip->getDate().toString();
        statline[tr("Vuosi")] = QString::number(trip->getDate().year());
        statline[tr("Kuukausi")] = QString::number(trip->getDate().month());
        statline[tr("Vuosi ja kk")] = QString::number(trip->getDate().year())+" "+QString::number(trip->getDate().month());

        if(trip->getTime().second.hour() - trip->getTime().first.hour() > 0)
            statline[tr("Reissun pituus")] = QString::number(trip->getTime().second.hour() - trip->getTime().first.hour());
        else
            statline[tr("Reissun pituus")] = QString::number(24 + trip->getTime().second.hour() - trip->getTime().first.hour());

        statistics.push_back(statline);
    }

    return calculate(statistics);

}
