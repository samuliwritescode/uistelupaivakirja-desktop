#ifndef ROUTEINFO_H
#define ROUTEINFO_H
#include "trip.h"
#include "locationprovider.h"

class RouteInfo
{
public:
    RouteInfo(Trip*);
    QString toString();
    QDateTime startTime();
    QDateTime endTime();

    double trackDistance();
    TrackPoint nearestPoint(const QDateTime&);

private:
    double trackDistance(int, int);
    QList<TrackPoint> m_trackpts;
};

#endif // ROUTEINFO_H
