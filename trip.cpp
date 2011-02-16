#include <QDebug>
#include <QSettings>
#include "singletons.h"
#include "trip.h"
#include "gpxreader.h"

Trip::Trip():
    TrollingObject(),
    m_selectedFish(-1)
{
    setType("trip");
    set("date",QDate::currentDate());
    m_nullFish = new Fish();
}

Trip::~Trip()
{
    delete m_nullFish;
    m_nullFish = NULL;
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
    set("waypointfile", importFile(p_wpts));
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
    if(id < 0 && m_selectedFish >= 0 && m_selectedFish < m_catch.size())
    {
        return m_catch.at(m_selectedFish);
    }
    else if(id >= 0 && id < m_catch.size())
    {
        return m_catch.at(id);
    }

    return m_nullFish;
}

int Trip::getFishCount()
{
    return m_catch.size();
}

void Trip::selectFish(int id)
{
   m_selectedFish = id;
}

Fish* Trip::newFish(Fish::EType type)
{
    QSettings settings;
    Fish* fish = new Fish();
    if(type == Fish::eFish ||
       type == Fish::eFishAndWeather)
    {
        fish->setSpecies(settings.value("Species").toString());
        fish->setGetter(settings.value("Getter").toString());
        fish->setMethod(settings.value("Method").toString());
    }

    if(type == Fish::eWeather ||
       type == Fish::eFishAndWeather)
    {
        for(int loop=m_catch.count()-1; loop >= 0; loop--)
        {
            Fish* weather = m_catch.at(loop);
            if(weather->getType() == Fish::eWeather ||
               weather->getType() == Fish::eFishAndWeather)
            {
                fish->setAirTemp(weather->getAirTemp());
                fish->setWaterTemp(weather->getWaterTemp());
                fish->setWeatherCondition(weather->getWeatherCondition());
                fish->setWindCondition(weather->getWindCondition());
                fish->setPressureCondition(weather->getPressureCondition());
                fish->setWindDirection(weather->getWindDirection());
                fish->setPressureChange(weather->getPressureChange());
                break;
            }
        }
    }

    fish->setTime(get("time_start").toTime());
    connect(fish, SIGNAL(FishModified()), this, SLOT(FishModified()));
    fish->setType(type);
    m_catch.push_back(fish);
    m_selectedFish = m_catch.size()-1;
    return fish;
}

void Trip::deleteFish(int id)
{
    if(id > -1 && m_catch.size() > id)
    {
        Fish* fish = m_catch.at(id);
        disconnect(fish, SIGNAL(FishModified()), this, SLOT(FishModified()));
        FishModified();
        delete fish;
        m_catch.removeAt(id);
    }
}

int Trip::getSelectedFish()
{
    return m_selectedFish;
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

void Trip::FishModified()
{
    setUnsaved();
}

void Trip::constructItems(const TrollingObjectItemList& p_items)
{
    m_catch.clear();
    foreach(TrollingObjectItem item, p_items)
    {
        Fish* fish = new Fish();
        connect(fish, SIGNAL(FishModified()), this, SLOT(FishModified()));

        for(QHash<QString, QVariant>::iterator iter = item.begin(); iter != item.end(); iter++)
        {
            fish->m_properties[iter.key()] = iter.value();
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

        QList<QString> propnames = fish->getPropertyNames();
        foreach(QString propname, propnames)
        {
            fishprops[propname] = fish->getProperty(propname);
        }

        if(fish->getLure())
            fishprops["lure"] = fish->getLure()->getId();

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
