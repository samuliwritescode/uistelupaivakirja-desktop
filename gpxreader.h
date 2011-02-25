#ifndef GPXREADER_H
#define GPXREADER_H
#include "locationprovider.h"

class GPXReader: public LocationProvider
{
public:
    GPXReader();
    virtual void load(const QString& filename);
    virtual void clear();
    virtual QList<WayPoint> getWayPoints();
    virtual QList<TrackPoint> getTrackPoints();

private:
    QList<WayPoint> m_waypoints;
    QList<TrackPoint> m_trackpoints;
};

#endif // GPXREADER_H
