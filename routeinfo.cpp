#include <math.h>
#include "routeinfo.h"
#define TORADIANS (M_PI / 180.0)

RouteInfo::RouteInfo(Trip* p_trip)
{
    m_trackpts = p_trip->getRoute();
}

QString RouteInfo::toString()
{
    if(m_trackpts.size() < 2)
        return QString();

    QString retval;
    retval += QObject::tr("Aloitus: ");
    retval += startTime().toString("dd.MM hh:mm");
    retval += "\n";
    retval += QObject::tr("Lopetus: ");
    retval += endTime().toString("dd.MM hh:mm");
    retval += "\n";
    retval += QObject::tr("Reitin pituus: ");
    retval += QString::number(trackDistance());
    retval += " km";

    return retval;
}

double RouteInfo::trackDistance(int startindex, int endindex)
{
    double distance = 0;
    if(startindex < 0)
        startindex = 0;

    if(endindex >= m_trackpts.count()-1)
        endindex = m_trackpts.count()-2;

    if(endindex < 0)
        return 0;

    for(int loop=startindex; loop < endindex; loop++)
    {
        double lon1 = m_trackpts.at(loop).lon * TORADIANS;
        double lat1 = m_trackpts.at(loop).lat * TORADIANS;
        double lon2 = m_trackpts.at(loop+1).lon * TORADIANS;
        double lat2 = m_trackpts.at(loop+1).lat * TORADIANS;

        double dlon = lon2 - lon1;
        double dlat = lat2 - lat1;
        double a = pow(sin(dlat/2.0), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon/2.0), 2);
        distance += 6372.797 * 2 * atan2(sqrt(a), sqrt(1-a));
    }
    return distance;
}

double RouteInfo::trackDistance()
{
    return trackDistance(0, m_trackpts.count());
}

QDateTime RouteInfo::startTime()
{
    if(m_trackpts.count() > 0)
        return m_trackpts.first().time;

    return QDateTime();
}

QDateTime RouteInfo::endTime()
{
    if(m_trackpts.count() > 0)
        return m_trackpts.last().time;

    return QDateTime();
}
