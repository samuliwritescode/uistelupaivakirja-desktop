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
