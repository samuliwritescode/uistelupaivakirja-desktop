#include <QDebug>
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

double RouteInfo::speedAt(const QDateTime& p_time)
{
    int idx = nearestIndex(p_time);
    if(idx > 1 && idx < m_trackpts.count())
    {
        double distance = trackDistance(idx-1, idx);
        int time = m_trackpts[idx-1].time.secsTo(m_trackpts[idx].time);
        return distance/time*3600;
    }

    return 0;
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

int RouteInfo::nearestIndex(const QDateTime& p_time)
{
    int count = m_trackpts.count();
    int idx = count/2;
    int hop = ceil(count/4);
    int round = 2;
    while(idx >= 0 &&
          idx < count)
    {
      TrackPoint pt = m_trackpts.at(idx);
      if(p_time > pt.time)
          idx += hop;
      else
          idx -= hop;

      if(hop <= 1)
          break;

      round++;
      hop = ceil(count/pow(2, round));
    }

    return idx;
}

TrackPoint RouteInfo::nearestPoint(const QDateTime& p_time)
{
    int idx = nearestIndex(p_time);

    if(idx > 0 &&
       idx < m_trackpts.count() &&
       abs(p_time.secsTo(m_trackpts.at(idx).time)) < 60)
    {
        return m_trackpts.at(idx);
    }

    return TrackPoint();
}

double RouteInfo::trackDistance()
{
    return trackDistance(0, m_trackpts.count());
}

QDateTime RouteInfo::startTime()
{
    if(m_trackpts.count() > 0)
    {
        //This is workaround hack for such event that
        //route log has older log. Filter it out by traversing
        //through and ignoring beginning if there is gap over
        //one hour.
        for(int loop=0; loop < m_trackpts.count()-1; loop++)
        {
           if(m_trackpts[loop].time.secsTo(m_trackpts[loop+1].time) < 3600)
               continue;

           return m_trackpts[loop+1].time;
        }
        return m_trackpts.first().time;
    }

    return QDateTime();
}

QDateTime RouteInfo::endTime()
{
    if(m_trackpts.count() > 0)
        return m_trackpts.last().time;

    return QDateTime();
}
