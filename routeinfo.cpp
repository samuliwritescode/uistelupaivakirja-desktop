#include "routeinfo.h"

RouteInfo::RouteInfo(Trip* p_trip)
{
    m_trackpts = p_trip->getRoute();
}

QString RouteInfo::toString()
{
    return QObject::tr("points: ")+QString::number(m_trackpts.count());
}
