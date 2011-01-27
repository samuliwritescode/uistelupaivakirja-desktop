#include <QDebug>
#include "singletons.h"
#include "species.h"
#include "trip.h"

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

QList< QMap<QString, QVariant> > Trip::getList()
{
    QList< QMap<QString, QVariant> > retval;
    for(int loop=0; loop < m_catch.size(); loop++)
    {
        QMap<QString, QVariant> fishprops;
        Fish* fish = m_catch.at(loop);

        if(fish->getSpecies())
            fishprops["species"] = fish->getSpecies()->getId();

        if(fish->getMethod())
            fishprops["method"] = fish->getMethod()->getId();

        if(fish->getLure())
            fishprops["lure"] = fish->getLure()->getId();

        fishprops["weight"] = fish->getWeight();
        fishprops["length"] = fish->getLength();
        fishprops["depth"] = fish->getDepth();
        retval.push_back(fishprops);
    }
    return retval;
}

void Trip::storeList(QList< QMap<QString, QVariant> > p_list)
{
    for(int loop=0; loop < p_list.size(); loop++)
    {
        QMap<QString, QVariant> fishprops = p_list.at(loop);
        Fish* fish = new Fish();        
        fish->setWeight(fishprops["weight"].toDouble());
        fish->setLength(fishprops["length"].toDouble());
        fish->setDepth(fishprops["depth"].toDouble());
        Species* species = Singletons::model()->getSpecies(fishprops["species"].toInt());
        if(species)
            fish->setSpecies(species);

        Method* method = Singletons::model()->getMethod(fishprops["method"].toInt());
        if(method)
            fish->setMethod(method);

        Lure* lure = Singletons::model()->getLure(fishprops["lure"].toInt());
        if(lure)
            fish->setLure(lure);

        m_catch.push_back(fish);
    }
}

void Trip::setWaterTemp(double temp)
{
    set("watertemp", temp);
}

double Trip::getWaterTemp()
{
    return get("watertemp").toDouble();
}


void Trip::setDescription(const QString& desc)
{
    set("description", desc);
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

void Trip::addWindCondition(EWindCondition wind, bool bSet)
{
    int mask = get("wind_condition").toInt();
    if(bSet)
        mask = mask|wind;
    else
        mask = mask^wind;

    set("wind_condition", mask);
}

bool Trip::isWindCondition(EWindCondition wind)
{
   int mask = get("wind_condition").toInt();
   return (mask&wind)!=0;
}

void Trip::addWeatherCondition(EWeatherCondition weather, bool bSet)
{
    int mask = get("weather_condition").toInt();
    if(bSet)
        mask = mask|weather;
    else
        mask = mask^weather;

    set("weather_condition", mask);
}

bool Trip::isWeatherCondition(EWeatherCondition weather)
{
    int mask = get("weather_condition").toInt();
    return (mask&weather)!=0;
}


