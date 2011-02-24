#ifndef LOCATIONPROVIDER_H
#define LOCATIONPROVIDER_H
#include <QList>
#include <QDateTime>

class WayPoint
{
public:
    double lat;
    double lon;
    QDateTime time;
    QString name;
};

class TrackPoint
{
public:
    TrackPoint() {lat=0; lon=0; time=QDateTime();}
    double lat;
    double lon;
    QDateTime time;
};

class LocationProvider
{
public:
    virtual QList<WayPoint> getWayPoints() = 0;
    virtual QList<TrackPoint> getTrackPoints() = 0;

protected:
    LocationProvider();

};

#endif // LOCATIONPROVIDER_H
