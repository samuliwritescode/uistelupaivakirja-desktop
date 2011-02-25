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
    double speedAt(const QDateTime&);

private:
    double trackDistance(int, int);
    int nearestIndex(const QDateTime&);
    QList<TrackPoint> m_trackpts;
};

#endif // ROUTEINFO_H
