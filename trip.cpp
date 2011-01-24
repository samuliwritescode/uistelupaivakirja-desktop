#include <QDebug>
#include "singletons.h"
#include "species.h"
#include "trip.h"

Trip::Trip():
    TrollingObject(),
    m_fish(NULL)
{
    m_type = "trip";
    m_properties["date"] = QDate::currentDate();
}

void Trip::setDate(const QDate& date)
{
    m_properties["date"] = date;
}

QDate Trip::getDate()
{
    return m_properties["date"].toDate();
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

void Trip::setFish(int id)
{
   m_fish = m_catch.at(id);
}

int Trip::getFishCount()
{
    return m_catch.size();
}

void Trip::addFish()
{
    if(!m_fish)
        getFish();

    m_catch.push_back(m_fish);
    m_fish = NULL;
}

void Trip::removeFish()
{
    for(int loop=0; loop < m_catch.size(); loop++)
    {
        if(m_catch[loop] == m_fish)
        {
            m_catch.removeAt(loop);
            delete m_fish;
            m_fish = NULL;
        }
    }

    if(m_fish)
    {
        delete m_fish;
        m_fish = NULL;
    }
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

        fishprops["method"] = fish->getMethod();
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
        fish->setMethod( static_cast<Fish::EMethod>(fishprops["method"].toInt()) );
        fish->setWeight(fishprops["weight"].toDouble());
        fish->setLength(fishprops["length"].toDouble());
        fish->setDepth(fishprops["depth"].toDouble());
        Species* species = Singletons::model()->getSpecies(fishprops["species"].toInt());
        if(species)
            fish->setSpecies(species);
        m_catch.push_back(fish);
    }
}

void Trip::setWaterTemp(double temp)
{
    m_properties["watertemp"] = temp;
}

double Trip::getWaterTemp()
{
    return m_properties["watertemp"].toDouble();
}


void Trip::setDescription(const QString& desc)
{
    m_properties["description"] = desc;
}


void Trip::setTime(const QTime& start, const QTime& end)
{
    if(!start.isNull())
        m_properties["time_start"] = start;

    if(!end.isNull())
        m_properties["time_end"] = end;
}

QPair<QTime, QTime> Trip::getTime()
{
    QPair<QTime, QTime> retval;
    retval.first = m_properties["time_start"].toTime();
    retval.second = m_properties["time_end"].toTime();
    return retval;
}

void Trip::addWindCondition(EWindCondition wind, bool bSet)
{
    int mask = m_properties["wind_condition"].toInt();
    if(bSet)
        mask = mask|wind;
    else
        mask = mask^wind;

    m_properties["wind_condition"] = mask;
}

bool Trip::isWindCondition(EWindCondition wind)
{
   int mask = m_properties["wind_condition"].toInt();
   return (mask&wind)!=0;
}

void Trip::addWeatherCondition(EWeatherCondition weather, bool bSet)
{
    int mask = m_properties["weather_condition"].toInt();
    if(bSet)
        mask = mask|weather;
    else
        mask = mask^weather;

    m_properties["weather_condition"] = mask;
}

bool Trip::isWeatherCondition(EWeatherCondition weather)
{
    int mask = m_properties["weather_condition"].toInt();
    return (mask&weather)!=0;
}


