#include <QDebug>
#include "trip.h"

Trip::Trip():
    TrollingObject()
{
    m_type = "trip";
}

void Trip::setDate(const QDate& date)
{
    m_properties["date"] = date;
}

QDate Trip::getDate()
{
    return m_properties["date"].toDate();
}

Fish* Trip::getFish(int id)
{
    if(id < 0)
        return new Fish();

    return NULL;
}

void Trip::setWaterTemp(double temp)
{
    m_properties["watertemp"] = temp;
}

double Trip::getWaterTemp()
{
    return m_properties["watertemp"].toDouble();
}


void Trip::setDescription(const QString& desc)
{
    m_properties["description"] = desc;
}


void Trip::setTime(const QTime& start, const QTime& end)
{
    if(!start.isNull())
        m_properties["time_start"] = start;

    if(!end.isNull())
        m_properties["time_end"] = end;
}

QPair<QTime, QTime> Trip::getTime()
{
    QPair<QTime, QTime> retval;
    retval.first = m_properties["time_start"].toTime();
    retval.second = m_properties["time_end"].toTime();
    return retval;
}

void Trip::addWindCondition(EWindCondition wind, bool bSet)
{
    int mask = m_properties["wind_condition"].toInt();
    if(bSet)
        mask = mask|wind;
    else
        mask = mask^wind;

    m_properties["wind_condition"] = mask;
}

bool Trip::isWindCondition(EWindCondition wind)
{
   int mask = m_properties["wind_condition"].toInt();
   return (mask&wind)!=0;
}
