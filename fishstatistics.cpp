#include "fishstatistics.h"

#define COL_SPECIES tr("Laji")
#define COL_WEIGHT tr("Paino")
#define COL_LENGTH tr("Pituus")
#define COL_WATERDEPTH tr("Veden syvyys")
#define COL_LINEWEIGHT tr("Painotus")
#define COL_RELEASELENGTH tr("Vapautuspituus")
#define COL_SPOTDEPTH tr("Ottisyvyys")
#define COL_TIME tr("Aika")
#define COL_GETTER tr("Saaja")
#define COL_METHOD tr("Kalastustapa")
#define COL_LUREMANUFACTURER tr("Uistinvalmistaja")
#define COL_PLACE tr("Kalapaikka")
#define COL_DATE tr("Päiväys")
#define COL_YEAR tr("Vuosi")
#define COL_MONTH tr("Kuukausi")
#define COL_YEARMONTH tr("Vuosi ja kk")
#define COL_TRIPLENGTH tr("Reissun pituus")
#define COL_TEMP tr("Lämpötila")
#define COL_WEATHER tr("Säätila")
#define COL_WIND tr("Tuuli")
#define COL_WINDDIR tr("Tuulen suunta")
#define COL_PRESSURE tr("Ilmanpaine")
#define COL_PRESSURECHANGE tr("Ilmanpaineen muutos")

FishStatistics::FishStatistics(QObject *) :
    TrollingStatistics()
{
}

QStringList FishStatistics::getTextFields()
{
    QStringList retval;
    retval << COL_SPECIES;
    retval << COL_TIME;
    retval << COL_GETTER;
    retval << COL_METHOD;
    retval << COL_LUREMANUFACTURER;
    retval << COL_PLACE;
    retval << COL_DATE;
    retval << COL_YEAR;
    retval << COL_MONTH;
    retval << COL_YEARMONTH;
    retval << COL_WEATHER;
    retval << COL_WIND;
    retval << COL_WINDDIR;
    retval << COL_PRESSURE;
    retval << COL_PRESSURECHANGE;
    return retval;
}

QStringList FishStatistics::getNumericFields()
{
    QStringList retval;
    retval << COL_WEIGHT;
    retval << COL_LENGTH;
    retval << COL_WATERDEPTH;
    retval << COL_LINEWEIGHT;
    retval << COL_RELEASELENGTH;
    retval << COL_SPOTDEPTH;
    retval << COL_TEMP;
    return retval;
}

QString FishStatistics::getName()
{
    return tr("Kalat");
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
                statline[COL_SPECIES] = fish->getSpecies().toLower();
                statline[COL_WEIGHT] = fish->getWeight();
                statline[COL_LENGTH] = fish->getLength();
                statline[COL_WATERDEPTH] = fish->getTotalDepth();
                statline[COL_LINEWEIGHT] = fish->getLineWeight();
                statline[COL_RELEASELENGTH] = fish->getReleaseWidth();
                statline[COL_SPOTDEPTH] = fish->getSpotDepth();
                statline[COL_TIME] = fish->getTime().toString();
                statline[COL_GETTER] = fish->getGetter();
                statline[COL_METHOD] = fish->getMethod();

                if(fish->getLure())
                {
                    Lure* lure = fish->getLure();
                    statline[COL_LUREMANUFACTURER] = lure->getMaker();
                }

                statline[COL_PLACE] = trip->getPlace()->getName();
                statline[COL_DATE] = trip->getDate().toString();
                statline[COL_YEAR] = QString::number(trip->getDate().year());
                statline[COL_MONTH] = trip->getDate().toString("MM");
                statline[COL_YEARMONTH] = QString::number(trip->getDate().year())+" "+trip->getDate().toString("MM");

                if(trip->getTime().second.hour() - trip->getTime().first.hour() > 0)
                    statline[COL_TRIPLENGTH] = QString::number(trip->getTime().second.hour() - trip->getTime().first.hour());
                else
                    statline[COL_TRIPLENGTH] = QString::number(24 + trip->getTime().second.hour() - trip->getTime().first.hour());

                int fishCount = 0;
                for(int i=0; i<trip->getFishCount(); i++)
                {
                    Fish* other = trip->getFish(i);
                    if(other->getType() == Fish::eFish ||
                       other->getType() == Fish::eFishAndWeather)
                    {
                        fishCount++;
                    }
                }

                statline[tr("Aikaa per kala")] = QString::number(statline[COL_TRIPLENGTH].toDouble() / fishCount);

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

                statline[COL_TEMP] = fish->getAirTemp();
                statline[COL_WEATHER] = fish->getHumanReadableWeather();
                statline[COL_WIND] = fish->getHumanReadableWind();
                statline[COL_WINDDIR] = fish->getHumanReadableWindDirection();
                statline[COL_PRESSURE] = fish->getHumanReadablePressure();
                statline[COL_PRESSURECHANGE] = fish->getHumanReadablePressureChange();

                statistics.push_back(statline);
            }
        }
    }
    return calculate(statistics);
}
