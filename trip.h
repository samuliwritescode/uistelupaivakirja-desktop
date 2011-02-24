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

    QString getDescription();
    void setDescription(const QString&);

    void setWayPoints(const QString&);
    QList<WayPoint> getWayPoints();

    void setRoute(const QString&);
    QList<TrackPoint> getRoute();

    void setPlace(Place*);
    Place* getPlace();

    int getFishCount();
    Fish* getFish(int id = -1);

    void selectFish(int);
    Fish* newFish(Fish::EType type);
    void deleteFish(int);
    int getSelectedFish();
    virtual QString valid();

    void addMediaFile(const QString&);
    QStringList getMediaFiles();
    void removeMediaFile(const QString&);

protected:
    int indexOfFish(Fish*);

    virtual void constructItems(const TrollingObjectItemList&);
    virtual TrollingObjectItemList serializeItems();

private slots:
    void FishModified();

private:
    LocationProvider* getLocationReader();
    QList<Fish*> m_catch;
    int m_selectedFish;
    LocationProvider* m_reader;
};

#endif // TRIP_H
