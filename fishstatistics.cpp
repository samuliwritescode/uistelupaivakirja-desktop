#include "fishstatistics.h"

FishStatistics::FishStatistics(QObject *) :
    TrollingStatistics()
{
}


QMap<QString, QString> FishStatistics::stats()
{

    QList<QMap<QString, QString> > statistics;
    QMap<int, Trip*> trips = Singletons::model()->getTrips();
    foreach(Trip* trip, trips)
    {
        for(int loop=0; loop < trip->getFishCount(); loop++)
        {
            QMap<QString, QString> statline;
            Fish* fish = trip->getFish(loop);
            if(fish->getType() == Fish::eFish ||
               fish->getType() == Fish::eFishAndWeather)
            {
                statline[tr("Laji")] = fish->getProperty(FISH_SPECIES).toString();
                statline[tr("Paino")] = fish->getProperty(FISH_WEIGHT).toString();
                statline[tr("Pituus")] = fish->getProperty(FISH_LENGTH).toString();
                statline[tr("Veden syvyys")] = fish->getProperty(FISH_TOTAL_DEPTH).toString();
                statline[tr("Painotus")] = fish->getProperty(FISH_LINE_WEIGHT).toString();
                statline[tr("Vapautuspituus")] = fish->getProperty(FISH_RELEASE_WIDTH).toString();
                statline[tr("Ottisyvyys")] = fish->getProperty(FISH_SPOT_DEPTH).toString();
                statline[tr("Aika")] = fish->getProperty(FISH_TIME).toString();
                statline[tr("Saaja")] = fish->getProperty(FISH_GETTER).toString();
                statline[tr("Kalastustapa")] = fish->getProperty(FISH_METHOD).toString();

                if(fish->getLure())
                {
                    Lure* lure = fish->getLure();
                    statline[tr("Uistinvalmistaja")] = lure->getMaker();
                }

                statline[tr("Kalapaikka")] = trip->getPlace()->getName();
                statline[tr("Päiväys")] = trip->getDate().toString();
                statline[tr("Vuosi")] = QString::number(trip->getDate().year());
                statline[tr("Kuukausi")] = QString::number(trip->getDate().month());
                statline[tr("Vuosi ja kk")] = QString::number(trip->getDate().year())+" "+QString::number(trip->getDate().month());

                if(trip->getTime().second.hour() - trip->getTime().first.hour() > 0)
                    statline[tr("Reissun pituus")] = QString::number(trip->getTime().second.hour() - trip->getTime().first.hour());
                else
                    statline[tr("Reissun pituus")] = QString::number(24 + trip->getTime().second.hour() - trip->getTime().first.hour());

                //Kalarivit
                statline[tr("Säätila")] = QString::number(fish->getProperty(FISH_WEATHER).toInt());

                statistics.push_back(statline);
            }
        }
    }
    return calculate(statistics);
}
