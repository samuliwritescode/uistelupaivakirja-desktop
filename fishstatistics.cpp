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
                statline[tr("Laji")] = fish->getSpecies();
                statline[tr("Paino")] = fish->getWeight();
                statline[tr("Pituus")] = fish->getLength();
                statline[tr("Veden syvyys")] = fish->getTotalDepth();
                statline[tr("Painotus")] = fish->getLineWeight();
                statline[tr("Vapautuspituus")] = fish->getReleaseWidth();
                statline[tr("Ottisyvyys")] = fish->getSpotDepth();
                statline[tr("Aika")] = fish->getTime().toString();
                statline[tr("Saaja")] = fish->getGetter();
                statline[tr("Kalastustapa")] = fish->getMethod();

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

                //No weather information. Look for closest
                if(fish->getType() == Fish::eFish)
                {
                    Fish* closestHit = NULL;
                    int closestTime = 0xFFFFFF;
                    for(int i=0; i < trip->getFishCount(); i++)
                    {
                        Fish* weather = trip->getFish(i);
                        if(weather->getType() == Fish::eWeather ||
                           weather->getType() == Fish::eFishAndWeather)
                        {
                            QTime time1 = weather->getTime();
                            QTime time2 = fish->getTime();
                            if(abs(time1.secsTo(time2)) < closestTime)
                            {
                                closestTime = abs(time1.secsTo(time2));
                                closestHit = weather;
                            }
                        }
                    }

                    if(closestHit)
                    {
                        fish = closestHit;
                    }
                }

                statline[tr("Lämpötila")] = fish->getAirTemp();
                statline[tr("Säätila")] = fish->getHumanReadableWeather();
                statline[tr("Tuuli")] = fish->getHumanReadableWind();
                statline[tr("Tuulen suunta")] = fish->getHumanReadableWindDirection();
                statline[tr("Ilmanpaine")] = fish->getHumanReadablePressure();
                statline[tr("Ilmanpaineen muutos")] = fish->getHumanReadablePressureChange();

                statistics.push_back(statline);
            }
        }
    }
    return calculate(statistics);
}
