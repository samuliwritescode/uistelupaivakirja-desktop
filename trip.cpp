#include <QDebug>
#include "singletons.h"
#include "trip.h"
#include "gpxreader.h"

Trip::Trip():
    TrollingObject(),
    m_fish(NULL)
{
    setType("trip");
    set("date",QDate::currentDate());
}

void Trip::setDate(const QDate& date)
{
    set("date", date);
}

QDate Trip::getDate()
{
    return get("date").toDate();
}

void Trip::setTime(const QTime& start, const QTime& end)
{
    if(!start.isNull())
        set("time_start", start);

    if(!end.isNull())
        set("time_end", end);
}

QPair<QTime, QTime> Trip::getTime()
{
    QPair<QTime, QTime> retval;
    retval.first = get("time_start").toTime();
    retval.second = get("time_end").toTime();
    return retval;
}

void Trip::setWayPoints(const QString& p_wpts)
{
    set("waypointfile", p_wpts);
}

QList<WayPoint> Trip::getWayPoints()
{
    if(get("waypointfile").toString().isEmpty())
        return QList<WayPoint>();
    GPXReader reader;
    reader.load(get("waypointfile").toString());
    return reader.getWayPoints();
}

Fish* Trip::getFish(int id)
{
    if(!m_fish)
    {
        m_fish = new Fish();
    }

    if(id < 0)
    {
        return m_fish;
    }

    return m_catch.at(id);
}

int Trip::getFishCount()
{
    return m_catch.size();
}

void Trip::selectFish(int id)
{
   m_fish = m_catch.at(id);
}

Fish* Trip::newFish()
{
    if(indexOfFish(m_fish) == -1)
    {
        delete m_fish;
    }

    m_fish = new Fish();
    return m_fish;
}

void Trip::insertFish()
{
    if(indexOfFish(m_fish) == -1)
    {
        m_catch.push_back(m_fish);
    }
    newFish();
}

void Trip::deleteFish(int id)
{
    if(indexOfFish(m_fish) == id)
    {
        newFish();
    }

    if(id > -1 && m_catch.size() > id)
    {
        delete m_catch.at(id);
        m_catch.removeAt(id);
    }
}

int Trip::getSelectedFish()
{
    return indexOfFish(m_fish);
}

int Trip::indexOfFish(Fish* p_fish)
{
    for(int loop=0; loop < m_catch.size(); loop++)
    {
        if(m_catch.at(loop) == p_fish)
        {
            return loop;
        }
    }
    return -1;
}

void Trip::constructItems(const TrollingObjectItemList& p_items)
{
    m_catch.clear();
    foreach(TrollingObjectItem item, p_items)
    {
        Fish* fish = new Fish();

        for(QHash<QString, QVariant>::iterator iter = item.begin(); iter != item.end(); iter++)
        {
            fish->setProperty(iter.key(), iter.value());
        }

        Lure* lure = Singletons::model()->getLure(item["lure"].toInt());
        if(lure)
            fish->setLure(lure);

        m_catch.push_back(fish);
    }
}

TrollingObjectItemList Trip::serializeItems()
{
    TrollingObjectItemList retval;
    for(int loop=0; loop < m_catch.size(); loop++)
    {
        TrollingObjectItem fishprops;
        Fish* fish = m_catch.at(loop);

        if(fish->getLure())
            fishprops["lure"] = fish->getLure()->getId();

        QList<QString> propnames = fish->getPropertyNames();
        foreach(QString propname, propnames)
        {
            fishprops[propname] = fish->getProperty(propname);
        }

        retval.push_back(fishprops);
    }
    return retval;
}


void Trip::setPlace(Place* p_place)
{
    set("place", p_place->getId());
}

Place* Trip::getPlace()
{
    Place* place = Singletons::model()->getPlace(get("place").toInt());
    return place;
}
