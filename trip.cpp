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
    m_reader = NULL;
}

Trip::~Trip()
{
    if(m_reader)
        delete m_reader;

    foreach(Fish* fish, m_catch)
    {
        delete fish;
    }
    m_catch.clear();
}

void Trip::setDate(const QDate& date)
{
    set("date", date);
}

QDate Trip::getDate()
{
    return get("date").toDate();
}

QString Trip::getDescription()
{
    return get("description").toString();
}

void Trip::setDescription(const QString& description)
{
    set("description", description);
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

void Trip::setRoute(const QString& p_route)
{
    if(p_route.isEmpty())
    {
        set("routefile", QVariant());
        getLocationReader()->clear();
        return;
    }

    set("routefile", p_route);
    getLocationReader()->load(get("routefile").toString());
}

QList<TrackPoint> Trip::getRoute()
{
    if(get("routefile").toString().isEmpty())
        return QList<TrackPoint>();

    return getLocationReader()->getTrackPoints();
}

LocationProvider* Trip::getLocationReader()
{
    if(!m_reader)
    {
        m_reader = new GPXReader();
        m_reader->load(get("routefile").toString());
    }
    return m_reader;
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

    return NULL;
}

int Trip::getFishCount(Fish::EType type)
{
    if(type == Fish::eNaN)
        return m_catch.size();

    int count = 0;
    for(int loop=0; loop < m_catch.count(); loop++)
    {
        Fish::EType fishtype = m_catch.at(loop)->getType();
        if(fishtype == type)
        {
            if(m_catch.at(loop)->isGroup())
                count += m_catch.at(loop)->getGroupAmount();
            else
                count++;
        }
    }
    return count;
}

void Trip::selectFish(int id)
{
   m_selectedFish = id;
}

Fish* Trip::newFish(Fish::EType type)
{
    QSettings settings;
    Fish* fish = new Fish(this);
    if(settings.value("use_suggestions").toBool())
    {
        if(type == Fish::eFish ||
           type == Fish::eFishAndWeather)
        {
            fish->setSpecies(settings.value("Species").toString());
            fish->setGetter(settings.value("Getter").toString());
            fish->setMethod(settings.value("Method").toString());
            fish->setTrollingSpeed(settings.value("TrollingSpeed").toString());
            fish->setReleaseWidth(settings.value("ReleaseWidth").toString());
            fish->setLineWeight(settings.value("LineWeight").toString());
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
                    fish->setRainCondition(weather->getRainCondition());
                    break;
                }
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
    foreach(Fish* fish, m_catch)
    {
        delete fish;
    }

    m_catch.clear();
    foreach(TrollingObjectItem item, p_items)
    {
        Fish* fish = new Fish(this);
        connect(fish, SIGNAL(FishModified()), this, SLOT(FishModified()));

        for(QHash<QString, QString>::iterator iter = item.begin(); iter != item.end(); iter++)
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
            fishprops[propname] = fish->getProperty(propname).toString();
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

QString Trip::valid()
{
    if(getPlace() == NULL)
    {
        return tr("Reissulla ei ole kalapaikkaa");
    }
    else if(getTime().first.isNull())
    {
        return tr("Reissun aloitusaika puuttuu");
    }
    else if(getTime().second.isNull())
    {
        return tr("Reissun lopetusaika puuttuu");
    }
    foreach(Fish* fish, m_catch)
    {
        if(fish->getType() == Fish::eFish ||
           fish->getType() == Fish::eFishAndWeather)
        {
            if(fish->getSpecies().isEmpty())
            {
                return tr("Laji puuttuu kalalta");
            }

            if(fish->isGroup() && fish->getGroupAmount() <= 0)
            {
                return tr("Määrittelit kalan ryhmäksi, mutta kappalemäärä ei ole positiivinen luku");
            }
        }
        if(!fish->getTime().isValid())
        {
            return tr("Aika puuttuu kala/säätapahtumalta");
        }
    }

    return TrollingObject::valid();
}

void Trip::addMediaFile(const QString& p_file)
{
    QStringList files = get("mediafiles").toString().split("\n", QString::SkipEmptyParts);
    if(files.contains(p_file))
        return;

    files.append(p_file);
    set("mediafiles", files.join("\n"));
}

QStringList Trip::getMediaFiles()
{
    return get("mediafiles").toString().split("\n", QString::SkipEmptyParts);
}

void Trip::removeMediaFile(const QString& p_file)
{
    QStringList files = get("mediafiles").toString().split("\n", QString::SkipEmptyParts);
    for(int loop=0; loop < files.count(); loop++)
    {
        QString file = files.at(loop);
        if(file == p_file)
        {
            files.removeAt(loop);
            set("mediafiles", files.join("\n"));
            break;
        }
    }
}

QString Trip::toString()
{
    QString name = getDate().toString("yyyy-MM-dd");
    name += " ";
    name += getTime().first.toString("HH");
    name += "-";
    name += getTime().second.toString("HH");

    if(getPlace())
    {
        name += ", ";
        name += getPlace()->getName();
    }

    return name;
}

