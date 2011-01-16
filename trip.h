#ifndef TRIP_H
#define TRIP_H
#include <QDate>
#include "fish.h"
#include "trollingobject.h"

class Trip : public TrollingObject
{
public:
    Trip();

    void setWaterTemp(double temp);
    double getWaterTemp();
    void setDate(const QDate& date);
    QDate getDate();
    Fish* getFish(int id = -1);
};

#endif // TRIP_H
