#include "placecontroller.h"

PlaceController::PlaceController(QObject *parent) :
    QObject(parent)
{
}

QStringList PlaceController::getPlaces()
{
    QStringList list;
    list.append("Ristinselkä");
    list.append("Vanhanselkä");
    return list;
}
