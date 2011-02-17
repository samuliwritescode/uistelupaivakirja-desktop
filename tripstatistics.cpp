#include "tripstatistics.h"

#define COL_FISHCOUNT tr("Kaloja")
#define COL_PLACE tr("Kalapaikka")
#define COL_DATE tr("Päiväys")
#define COL_YEAR tr("Vuosi")
#define COL_MONTH tr("Kuukausi")
#define COL_YEARMONTH tr("Vuosi ja kk")
#define COL_TRIPLEN tr("Reissun pituus")

TripStatistics::TripStatistics(QObject *) :
    TrollingStatistics()
{
}

QStringList TripStatistics::getTextFields()
{
    QStringList retval;
    retval << COL_PLACE;
    retval << COL_DATE;
    retval << COL_YEAR;
    retval << COL_MONTH;
    retval << COL_YEARMONTH;
    retval.sort();
    return retval;
}

QStringList TripStatistics::getNumericFields()
{
    QStringList retval;
    retval << COL_FISHCOUNT;
    retval << COL_TRIPLEN;
    retval.sort();
    return retval;
}

QString TripStatistics::getName()
{
    return tr("Reissut");
}

QHash<QString, QString> TripStatistics::stats()
{
    QList<QHash<QString, QString> > statistics;
    QMap<int, Trip*> trips = Singletons::model()->getTrips();
    foreach(Trip* trip, trips)
    {
        int fishCount = 0;
        for(int loop=0; loop < trip->getFishCount(); loop++)
        {
            Fish* fish = trip->getFish(loop);
            if(fish->getType() == Fish::eFish ||
               fish->getType() == Fish::eFishAndWeather)
            {
                if(fish->isGroup() && fish->getGroupAmount() > 0)
                    fishCount += fish->getGroupAmount();
                else
                    fishCount++;
            }
        }

        QHash<QString, QString> statline;
        statline[COL_FISHCOUNT] = QString::number(fishCount);
        statline[COL_PLACE] = trip->getPlace()->getName();
        statline[COL_DATE] = trip->getDate().toString();
        statline[COL_YEAR] = QString::number(trip->getDate().year());
        statline[COL_MONTH] = trip->getDate().toString("MM");
        statline[COL_YEARMONTH] = QString::number(trip->getDate().year())+" "+trip->getDate().toString("MM");

        if(trip->getTime().second.hour() - trip->getTime().first.hour() > 0)
            statline[COL_TRIPLEN] = QString::number(trip->getTime().second.hour() - trip->getTime().first.hour());
        else
            statline[COL_TRIPLEN] = QString::number(24 + trip->getTime().second.hour() - trip->getTime().first.hour());

        statline[tr("Aikaa per kala")] = QString::number(statline[COL_TRIPLEN].toDouble() / fishCount);

        bool bSkip = false;
        for(QHash<QString, QString>::iterator iter = m_filters.begin(); iter != m_filters.end(); iter++)
        {
            if(statline.contains(iter.key()))
            {
                if(statline[iter.key()] != iter.value())
                {
                    bSkip=true;
                }
            }
            else
            {
                bSkip=true; //No match
            }
        }
        if(bSkip)
            continue;
        statistics.push_back(statline);
    }

    return calculate(statistics);

}
