#include <QDebug>
#include "tripcontroller.h"

TripController::TripController(QObject *parent) :
    QObject(parent)
{
}


void TripController::setDate(const QDate& date)
{
    qDebug() << "date was set to" << date;
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

QStringList TripController::getPlaces()
{
    QStringList list;
    list.append("Ristinselkä");
    list.append("Vanhanselkä");
    return list;
}
