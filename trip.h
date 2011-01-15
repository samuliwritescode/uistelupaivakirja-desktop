#ifndef TRIP_H
#define TRIP_H
#include <QDate>
#include "trollingobject.h"

class Trip : public TrollingObject
{
public:
    Trip();
    void setDate(const QDate& date);
    QDate getDate();
};

#endif // TRIP_H
