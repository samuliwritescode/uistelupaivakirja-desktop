#ifndef ROUTEINFO_H
#define ROUTEINFO_H
#include "trip.h"

class RouteInfo
{
public:
    RouteInfo(Trip*);
    QString toString();
    QDateTime startTime();
    QDateTime endTime();
    double trackDistance();

private:
    double trackDistance(int, int);
    QList<TrackPoint> m_trackpts;
};

#endif // ROUTEINFO_H
