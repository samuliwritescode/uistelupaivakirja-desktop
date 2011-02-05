#include <QDebug>
#include "singletons.h"
#include "trollingstatistics.h"

TrollingStatistics::TrollingStatistics():
        m_unit(TrollingStatistics::eCount)
{
}

void TrollingStatistics::setY(const QString& p_y)
{
    m_Y = p_y;
}

void TrollingStatistics::setUnit(EUnit p_unit)
{
    m_unit = p_unit;
}

QMap<QString, QString> TrollingStatistics::stats()
{
    QMap<QString, QString> retval;

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

    QMap<QString, double> fishcount;
    if(m_unit == TrollingStatistics::eCount)
    {
        fishcount = countFields(statistics, m_Y);
    }else if(m_unit == TrollingStatistics::eFishPerTime)
    {
        QMap<QString, double> count = countFields(statistics, m_Y);
        QMap<QString, double> time = sumFields(statistics, tr("Reissun pituus"));
        for(QMap<QString, double>::iterator iter= count.begin(); iter!=count.end(); iter++)
        {
            fishcount[iter.key()] = count[iter.key()] / time[iter.key()];
        }
    }


    for(QMap<QString, double>::iterator iter= fishcount.begin(); iter!=fishcount.end(); iter++)
    {
        retval[iter.key()] = QString::number(iter.value());
    }
    return retval;
}

QMap<QString, double> TrollingStatistics::countFields(const QList<QMap<QString, QString> >& statistics, const QString& field)
{
    QMap<QString, double> retval;
    for(int loop=0; loop < statistics.count(); loop++)
    {
        QMap<QString, QString> statline = statistics.at(loop);
        retval[statline[field]]++;
    }
    return retval;
}

QMap<QString, double> TrollingStatistics::sumFields(const QList<QMap<QString, QString> >& statistics, const QString& field)
{
    QMap<QString, double> retval;
    for(int loop=0; loop < statistics.count(); loop++)
    {
        QMap<QString, QString> statline = statistics.at(loop);
        retval[statline[m_Y]] = retval[statline[m_Y]] + statline[field].toDouble();
    }
    return retval;
}
