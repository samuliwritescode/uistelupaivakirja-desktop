#include <QDebug>
#include "trip.h"

Trip::Trip():
    TrollingObject()
{
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


void Trip::setTimeMask(int mask, bool value)
{
    int oldmask = m_properties["timemask"].toInt();
    int newmask = 0;
    if(value)
        newmask = oldmask|mask;
    else
        newmask = oldmask^mask;
    m_properties["timemask"] = newmask;

}

int Trip::getTimeMask()
{
    return m_properties["timemask"].toInt();
}
