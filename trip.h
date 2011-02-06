#ifndef TRIP_H
#define TRIP_H
#include <QDate>
#include <QTime>
#include "fish.h"
#include "place.h"
#include "trollingobject.h"
#include "locationprovider.h"

class Trip : public TrollingObject
{
    Q_OBJECT
public:
    Trip();
    ~Trip();

    void setDate(const QDate& date);
    QDate getDate();
    void setTime(const QTime& start, const QTime& end);
    QPair<QTime, QTime> getTime();

    void setWayPoints(const QString&);
    QList<WayPoint> getWayPoints();

    void setPlace(Place*);
    Place* getPlace();

    int getFishCount();
    Fish* getFish(int id = -1);

    void selectFish(int);
    Fish* newFish(Fish::EType type);
    void deleteFish(int);
    int getSelectedFish();

protected:
    int indexOfFish(Fish*);

    virtual void constructItems(const TrollingObjectItemList&);
    virtual TrollingObjectItemList serializeItems();

private slots:
    void FishModified();

private:
    QList<Fish*> m_catch;
    int m_selectedFish;
    Fish* m_nullFish;
};

#endif // TRIP_H
