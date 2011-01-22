#ifndef TRIP_H
#define TRIP_H
#include <QDate>
#include <QTime>
#include "fish.h"
#include "trollingobject.h"

class Trip : public TrollingObject
{
public:
    Trip();

    enum EWindCondition{eCalm=1, eFaint=2, eModerate=4, eBrisk=8, eHard=16};

    void setWaterTemp(double temp);
    double getWaterTemp();
    void setDate(const QDate& date);
    void setDescription(const QString& desc);
    QDate getDate();
    Fish* getFish(int id = -1);
    void setTime(const QTime& start, const QTime& end);
    QPair<QTime, QTime> getTime();
    void addWindCondition(EWindCondition wind, bool bSet);
    bool isWindCondition(EWindCondition wind);


};

#endif // TRIP_H
