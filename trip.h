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
    void setDescription(const QString& desc);
    QDate getDate();
    Fish* getFish(int id = -1);
    void setTimeMask(int mask, bool value);
    int getTimeMask();


};

#endif // TRIP_H
