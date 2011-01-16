#include <QDebug>
#include "singletons.h"
#include "tripcontroller.h"
#include "trollingmodel.h"

TripController::TripController(QObject *parent) :
        QObject(parent),
        m_trip(NULL)
{
    m_trip = Singletons::model()->getTrip();
}


void TripController::setDate(const QDate& date)
{
    if(m_trip)
        m_trip->setDate(date);
}

void TripController::addTime(int start, int end)
{
    qDebug() << "add time"<<start << end;
}

void TripController::removeTime(int start,int end)
{
    qDebug() << "remove time"<<start<<end;
}

void TripController::setPlace(const QString& place)
{
    qDebug() << "set place" << place;
}

void TripController::saveTrip()
{
    int newId = Singletons::model()->commit(m_trip);
    m_trip = Singletons::model()->getTrip(newId);
}


void TripController::setWaterTemp(const QString& temp)
{
    if(m_trip)
        m_trip->setWaterTemp(temp.toDouble());
}
