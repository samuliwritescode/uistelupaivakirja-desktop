#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include <QFile>
#include <QDebug>
#include "gpxreader.h"

GPXReader::GPXReader()
{
}

void GPXReader::load(const QString& filename)
{
    m_waypoints.clear();
    m_trackpoints.clear();
    QFile file(filename);
    QDomDocument document;
    if(file.open(QIODevice::ReadOnly))
    {
        QString error;
        int errorLine = 0;
        int errorColumn = 0;

        if(!document.setContent(&file, &error, &errorLine, &errorColumn))
        {
            qDebug() << "Cant' read from document" << error << errorLine << " " << errorColumn;
            file.close();
            return;
        }
    }
    file.close();

    QDomNodeList waypointnodelist = document.elementsByTagName("wpt");
    for(int loop=0; loop < waypointnodelist.count(); loop++)
    {
        QDomNode waypointnode = waypointnodelist.at(loop);
        WayPoint waypt;
        waypt.lat = 0;
        waypt.lon = 0;

        if(waypointnode.isElement())
        {
            QDomElement waypointelement = waypointnode.toElement();

            waypt.lat = waypointelement.attribute("lat").toDouble();
            waypt.lon = waypointelement.attribute("lon").toDouble();
        } else
            continue;

        QDomNodeList waypointchildnodes = waypointnode.childNodes();
        for(int loop2=0; loop2 < waypointchildnodes.count(); loop2++)
        {
            QDomNode waypointchildnode = waypointchildnodes.at(loop2);
            if(waypointchildnode.isElement())
            {
                QDomElement waypointchildelement = waypointchildnode.toElement();
                if(waypointchildelement.tagName() == "time")
                {
                    QDateTime time = QDateTime::fromString(waypointchildelement.text(), Qt::ISODate);
                    time.setTimeSpec(Qt::UTC);
                    waypt.time = time.toLocalTime();
                }
                if(waypointchildelement.tagName() == "name")
                    waypt.name = waypointchildelement.text();
            }
        }
        m_waypoints.push_back(waypt);
    }

    QDomNodeList trackpointnodelist = document.elementsByTagName("trkpt");
    for(int loop=0; loop < trackpointnodelist.count(); loop++)
    {
        QDomNode trackpointnode = trackpointnodelist.at(loop);
        TrackPoint trkpt;
        trkpt.lat = 0;
        trkpt.lon = 0;

        if(trackpointnode.isElement())
        {
            QDomElement trackpointelement = trackpointnode.toElement();

            trkpt.lat = trackpointelement.attribute("lat").toDouble();
            trkpt.lon = trackpointelement.attribute("lon").toDouble();
        }
        else
            continue;

        QDomNodeList trackpointchildnodes = trackpointnode.childNodes();
        for(int loop2=0; loop2 < trackpointchildnodes.count(); loop2++)
        {
            QDomNode trackpointchildnode = trackpointchildnodes.at(loop2);
            if(trackpointchildnode.isElement())
            {
                QDomElement trackpointchildelement = trackpointchildnode.toElement();
                if(trackpointchildelement.tagName() == "time")
                {
                    QDateTime time = QDateTime::fromString(trackpointchildelement.text(), Qt::ISODate);
                    time.setTimeSpec(Qt::UTC);
                    trkpt.time = time.toLocalTime();
                }
            }
        }
        m_trackpoints.push_back(trkpt);
    }
}

QList<WayPoint> GPXReader::getWayPoints()
{
    return m_waypoints;
}

QList<TrackPoint> GPXReader::getTrackPoints()
{
    return m_trackpoints;
}
