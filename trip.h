#ifndef TRIP_H
#define TRIP_H
#include <QDate>
#include <QTime>
#include "fish.h"
#include "site.h"
#include "trollingobject.h"

class Trip : public TrollingObject
{
public:
    Trip();



    void setDate(const QDate& date);
    QDate getDate();
    void setTime(const QTime& start, const QTime& end);
    QPair<QTime, QTime> getTime();

    void setSite(Site*);
    Site* getSite();

    int getFishCount();
    Fish* getFish(int id = -1);

    void selectFish(int);
    Fish* newFish();
    void deleteFish(int);
    void insertFish();
    int getSelectedFish();

protected:
    int indexOfFish(Fish*);

    virtual void constructItems(const TrollingObjectItemList&);
    virtual TrollingObjectItemList serializeItems();

private:
    QList<Fish*> m_catch;
    Fish* m_fish;
};

#endif // TRIP_H
