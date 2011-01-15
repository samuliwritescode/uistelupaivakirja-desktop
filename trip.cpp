#include <QDebug>
#include "trip.h"

Trip::Trip()
{
}

void Trip::setDate(const QDate& date)
{
    m_properties["date"] = date;
}

QDate Trip::getDate()
{
    return QDate(m_properties["date"].toDate());
}
