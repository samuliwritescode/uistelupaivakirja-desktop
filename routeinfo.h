#ifndef ROUTEINFO_H
#define ROUTEINFO_H
#include "trip.h"

class RouteInfo
{
public:
    RouteInfo(Trip*);
    QString toString();

private:
    QList<TrackPoint> m_trackpts;
};

#endif // ROUTEINFO_H
