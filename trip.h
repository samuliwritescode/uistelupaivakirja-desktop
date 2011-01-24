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
    enum EWeatherCondition{eClear=1, eHalfClear=2, eOvercast=4, eRain=8, eFog=16};

    void setWaterTemp(double temp);
    double getWaterTemp();
    void setDate(const QDate& date);
    void setDescription(const QString& desc);
    QDate getDate();
    void setTime(const QTime& start, const QTime& end);
    QPair<QTime, QTime> getTime();
    void addWindCondition(EWindCondition wind, bool bSet);
    bool isWindCondition(EWindCondition wind);

    void addWeatherCondition(EWeatherCondition weather, bool bSet);
    bool isWeatherCondition(EWeatherCondition weather);

    int getFishCount();
    Fish* getFish(int id = -1);

    void selectFish(int);
    Fish* newFish();
    void deleteFish(int);
    void insertFish();
    int getSelectedFish();


    virtual QList< QMap<QString, QVariant> > getList();
    virtual void storeList(QList< QMap<QString, QVariant> > p_list);

protected:
    int indexOfFish(Fish*);

private:
    QList<Fish*> m_catch;
    Fish* m_fish;

};

#endif // TRIP_H
