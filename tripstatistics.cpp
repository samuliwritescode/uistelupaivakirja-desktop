#include "tripstatistics.h"

#define COL_FISHCOUNT tr("Kaloja")
#define COL_PLACE tr("Kalapaikka")
#define COL_DATE tr("Päiväys")
#define COL_YEAR tr("Vuosi")
#define COL_MONTH tr("Kuukausi")
#define COL_YEARMONTH tr("Vuosi ja kk")
#define COL_TRIPLEN tr("Reissun pituus")


#define OPERATOR_FISHPERTIME tr("Kalaa tunnissa")
#define OPERATOR_TIMEPERFISH tr("Tuntia per kala")

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

QStringList TripStatistics::getOperators()
{
    QStringList retval = TrollingStatistics::getOperators();
    retval << OPERATOR_FISHPERTIME;
    retval << OPERATOR_TIMEPERFISH;
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
        if(trip->getPlace())
            statline[COL_PLACE] = trip->getPlace()->getName();
        else
            statline[COL_PLACE] = tr("n/a");

        statline[COL_DATE] = trip->getDate().toString();
        statline[COL_YEAR] = QString::number(trip->getDate().year());
        statline[COL_MONTH] = trip->getDate().toString("MM");
        statline[COL_YEARMONTH] = QString::number(trip->getDate().year())+" "+trip->getDate().toString("MM");

        if(trip->getTime().second.hour() - trip->getTime().first.hour() > 0)
            statline[COL_TRIPLEN] = QString::number(trip->getTime().second.hour() - trip->getTime().first.hour());
        else
            statline[COL_TRIPLEN] = QString::number(24 + trip->getTime().second.hour() - trip->getTime().first.hour());

        statline[tr("Aikaa per kala")] = QString::number(statline[COL_TRIPLEN].toDouble() / fishCount);

        if(!isMatch(statline))
            continue;

        statistics.push_back(statline);
    }

    return calculate(statistics);

}

QHash<QString, QString> TripStatistics::calculate(const QList<QHash<QString, QString> >& statistics)
{
    if(getOperator() == OPERATOR_FISHPERTIME ||
       getOperator() == OPERATOR_TIMEPERFISH)
    {
        QHash<QString, QString> retval;
        QHash<QString, double> triptime = sumFields(statistics, COL_TRIPLEN);
        QHash<QString, double> fishcount = sumFields(statistics, COL_FISHCOUNT);
        for(QHash<QString, double>::iterator iter = triptime.begin(); iter != triptime.end(); iter++)
        {
            fishcount[iter.key()] = fishcount[iter.key()] / triptime[iter.key()];
            if(getOperator() == OPERATOR_TIMEPERFISH && fishcount[iter.key()] != 0)
            {
                fishcount[iter.key()] = 1/fishcount[iter.key()];
            }
        }
        for(QHash<QString, double>::iterator iter= fishcount.begin(); iter!=fishcount.end(); iter++)
        {
            retval[iter.key()] = QString::number(iter.value());
        }

        return retval;
    } else
    {
        return TrollingStatistics::calculate(statistics);
    }
}
