#include <QDebug>
#include <QSettings>
#include "fish.h"
#include "trip.h"
#include "singletons.h"
#include "trollingexception.h"
#include "routeinfo.h"

Fish::Fish(Trip* p_parent):        
        m_parent(p_parent)
{
}

void Fish::setType(EType type)
{
    setProperty(FISH_TYPE, type);
}

Fish::EType Fish::getType()
{
    return static_cast<EType>(m_properties[FISH_TYPE].toInt());
}

void Fish::setLure(Lure* p_lure)
{
    setProperty(FISH_LURE, p_lure->getId());
}

Lure* Fish::getLure()
{
    Lure* lure = Singletons::model()->getLure(getProperty("lure").toInt());
    return lure;
}

void Fish::addMediaFile(const QString& p_file)
{
    QStringList files = getProperty(FISH_MEDIAFILES).toString().split("\n", QString::SkipEmptyParts);
    if(files.contains(p_file))
        return;

    files.append(p_file);
    setProperty(FISH_MEDIAFILES, files.join("\n"));
}

QStringList Fish::getMediaFiles()
{
    return getProperty(FISH_MEDIAFILES).toString().split("\n", QString::SkipEmptyParts);
}

void Fish::removeMediaFile(const QString& p_file)
{
    QStringList files = getProperty(FISH_MEDIAFILES).toString().split("\n", QString::SkipEmptyParts);
    for(int loop=0; loop < files.count(); loop++)
    {
        QString file = files.at(loop);
        if(file == p_file)
        {
            files.removeAt(loop);
            setProperty(FISH_MEDIAFILES, files.join("\n"));
            break;
        }
    }
}

void Fish::setUserField(const QString& p_field, const QString& p_value)
{
    QString userfield = getProperty(FISH_USERFIELD).toString();
    QStringList keyvalues = userfield.split("\n");
    for(int loop=0; loop < keyvalues.count(); loop++)
    {
        if( keyvalues.at(loop).split("=").count() > 1 )
        {
            QString key = keyvalues.at(loop).split("=").at(0);
            if(key == p_field)
            {
                keyvalues[loop] = key+"="+p_value;
                setProperty(FISH_USERFIELD, keyvalues.join("\n"));
                return;
            }
        }
    }
    setProperty(FISH_USERFIELD, userfield+"\n"+p_field+"="+p_value);
}

QMap<QString, QString> Fish::getUserFields()
{
    QMap<QString, QString> retval;
    QString userfield = getProperty(FISH_USERFIELD).toString();
    QStringList keyvalues = userfield.split("\n");
    for(int loop=0; loop < keyvalues.count(); loop++)
    {
        if( keyvalues.at(loop).split("=").count() > 1 )
        {
            QString key = keyvalues.at(loop).split("=").at(0);
            QString value;
            if(keyvalues.at(loop).split("=").count() == 2)
            {
                value = keyvalues.at(loop).split("=").at(1);
            }
            retval[key] = value;
        }
    }
    return retval;
}

QVariant Fish::getProperty(const QString& p_prop)
{
    return m_properties[p_prop];
}

void Fish::setProperty(const QString& p_prop, QVariant p_value)
{
    if(m_properties[p_prop].toString() == p_value.toString())
        return;

    m_properties[p_prop] = p_value;
    emit FishModified();
}

QList<QString> Fish::getPropertyNames()
{
    return m_properties.keys();
}

QString Fish::getHumanReadableWeather()
{
    int weather = getWeatherCondition();
    switch(weather)
    {
    case 1: return QObject::tr("Selkeää (0/8)");
    case 2: return QObject::tr("Selkeää (1/8)");
    case 3: return QObject::tr("Melko selkeää (2/8)");
    case 4: return QObject::tr("Puolipilvistä (3/8)");
    case 5: return QObject::tr("Puolipilvistä (4/8)");
    case 6: return QObject::tr("Melko pilvistä (5/8)");
    case 7: return QObject::tr("Melko pilvistä (6/8)");
    case 8: return QObject::tr("Pilvistä (7/8)");
    case 9: return QObject::tr("Pilvistä (8/8)");
    case 10: return QObject::tr("Sumua (9/8)");
    default: break;
    }
    return QObject::tr("n/a");
}

QString Fish::getHumanReadableWind()
{
    int wind = getWindCondition();
    switch(wind)
    {
    case 1: return QObject::tr("Tyyntä (0 m/s)");
    case 2: return QObject::tr("Heikkoa (1 - 3 m/s)");
    case 3: return QObject::tr("Kohtalaista (4 - 7 m/s)");
    case 4: return QObject::tr("Navakkaa (8 - 13 m/s)");
    case 5: return QObject::tr("Kovaa (14 - 20 m/s)");
    default: break;
    }
    return QObject::tr("n/a");
}

QString Fish::getHumanReadableRain()
{
    int rain = getRainCondition();
    switch(rain)
    {
    case 1: return QObject::tr("Pouta (<0.3 mm)");
    case 2: return QObject::tr("Vähän sadetta (0.3 - 0.9 mm)");
    case 3: return QObject::tr("Sadetta (1.0 - 4.4 mm)");
    case 4: return QObject::tr("Runsasta sadetta (>4.5 mm)");
    default: break;
    }
    return QObject::tr("n/a");
}

QString Fish::getHumanReadablePressure()
{
    int pressure = getPressureCondition();

    if(pressure == 0)
    {
        return QObject::tr("n/a");
    }
    else
    {
        return QString::number(pressure+940)+QObject::tr(" hPa");
    }
}

QString Fish::getHumanReadableWindDirection()
{
    EWindDirection direction = getWindDirection();
    switch(direction)
    {
    case eNoWindDirection: return QObject::tr("Ei suuntaa");
    case eSouth: return QObject::tr("Etelä");
    case eSouthWest: return QObject::tr("Lounas");
    case eWest: return QObject::tr("Länsi");
    case eNorthWest: return QObject::tr("Luode");
    case eNorth: return QObject::tr("Pohjoinen");
    case eNorthEast: return QObject::tr("Koillinen");
    case eEast: return QObject::tr("Itä");
    case eSouthEast: return QObject::tr("Kaakko");
    default: break;
    }
    return QObject::tr("n/a");
}

QString Fish::getHumanReadablePressureChange()
{
    EPressureChange change = getPressureChange();
    switch(change)
    {
    case eFastDecline: return QObject::tr("Laskee nopeasti");
    case eModerateDecline: return QObject::tr("Laskee");
    case eSlowDecline: return QObject::tr("Laskee hitaasti");
    case eNoChange: return QObject::tr("Muuttumaton");
    case eSlowRaise: return QObject::tr("Nousee hitaasti");
    case eModerateRaise: return QObject::tr("Nousee");
    case eFastRaise: return QObject::tr("Nousee nopeasti");

    default: break;
    }
    return QObject::tr("n/a");
}

void Fish::setDouble(const QString& p_prop, const QString& p_value)
{
    bool bConvOk = false;
    if(p_value.isEmpty())
    {
        setProperty(p_prop, QString());
    }
    else
    {
        QString value = p_value;
        value.replace(",", ".");
        if(value == "-")
            value = "-0";

        value.toDouble(&bConvOk);
        if(bConvOk)
        {
            setProperty(p_prop, value);
        }
        else
        {
            throw TrollingException("Value is not numeric: "+value+" property: "+p_prop);
        }
    }
}

void Fish::setWeight(const QString& p_val)
{
    setDouble(FISH_WEIGHT, p_val);
}

void Fish::setLength(const QString& p_val)
{
    setDouble(FISH_LENGTH, p_val);
}

void Fish::setSpotDepth(const QString& p_val)
{
    setDouble(FISH_SPOT_DEPTH, p_val);
}

void Fish::setTotalDepth(const QString& p_val)
{
    setDouble(FISH_TOTAL_DEPTH, p_val);
}

void Fish::setTrollingSpeed(const QString& p_val)
{
    QSettings settings;
    settings.setValue("TrollingSpeed", p_val.toLower());
    setDouble(FISH_TROLLING_SPEED, p_val);
}

void Fish::setLineWeight(const QString& p_val)
{
    QSettings settings;
    settings.setValue("LineWeight", p_val.toLower());
    setDouble(FISH_LINE_WEIGHT, p_val);
}

void Fish::setReleaseWidth(const QString& p_val)
{
    QSettings settings;
    settings.setValue("ReleaseWidth", p_val.toLower());
    setDouble(FISH_RELEASE_WIDTH, p_val);
}

void Fish::setSpecies(const QString& p_val)
{
    QSettings settings;
    settings.setValue("Species", p_val.toLower());
    setProperty(FISH_SPECIES, p_val);
}

void Fish::setMethod(const QString& p_val)
{
    QSettings settings;
    settings.setValue("Method", p_val.toLower());
    setProperty(FISH_METHOD, p_val);
}

void Fish::setGetter(const QString& p_val)
{
    QSettings settings;
    settings.setValue("Getter", p_val);
    setProperty(FISH_GETTER, p_val);
}

void Fish::setMiscText(const QString& p_val)
{
    setProperty(FISH_MISC_TEXT, p_val);
}

void Fish::setWaterTemp(const QString& p_val)
{
    setDouble(FISH_WATER_TEMP, p_val);
}

void Fish::setAirTemp(const QString& p_val)
{
    setDouble(FISH_AIR_TEMP, p_val);
}

void Fish::setCoordinates(const QString& p_lat, const QString& p_lon)
{
    setDouble(FISH_COORDINATES_LAT, p_lat);
    setDouble(FISH_COORDINATES_LON, p_lon);
}

void Fish::setTime(const QTime& p_val)
{
    setProperty(FISH_TIME, p_val);
}

void Fish::setWindCondition(int p_val)
{
    setProperty(FISH_WIND, p_val);
}

void Fish::setWindDirection(EWindDirection p_val)
{
    setProperty(FISH_WIND_DIRECTION, p_val);
}

void Fish::setWeatherCondition(int p_val)
{
    setProperty(FISH_WEATHER, p_val);
}

void Fish::setPressureCondition(int p_val)
{
    setProperty(FISH_PRESSURE, p_val);
}

void Fish::setRainCondition(int p_val)
{
    setProperty(FISH_RAIN, p_val);
}

void Fish::setPressureChange(EPressureChange p_val)
{
    setProperty(FISH_PRESSURE_CHANGE, p_val);
}

void Fish::setGroup(bool p_val)
{
    if(!p_val)
        setProperty(FISH_GROUP_AMOUNT, "");
    else
        setProperty(FISH_LENGTH, "");

    setProperty(FISH_IS_GROUP, p_val);
}

void Fish::setGroupAmount(int p_value)
{
    if(getProperty(FISH_IS_GROUP).toBool() == true)
        setProperty(FISH_GROUP_AMOUNT, p_value);
}

void Fish::setCR(bool p_val)
{
    setProperty(FISH_IS_CATCHRELEASED, p_val);
}

void Fish::setUnderSize(bool p_val)
{
    setProperty(FISH_IS_UNDERSIZE, p_val);
}

QString Fish::getWeight()
{
    return m_properties[FISH_WEIGHT].toString();
}

QString Fish::getLength()
{
    return m_properties[FISH_LENGTH].toString();
}

QString Fish::getSpotDepth()
{
    return m_properties[FISH_SPOT_DEPTH].toString();
}

QString Fish::getTotalDepth()
{
    return m_properties[FISH_TOTAL_DEPTH].toString();
}

QString Fish::getTrollingSpeed()
{
    return m_properties[FISH_TROLLING_SPEED].toString();
}

QString Fish::getLineWeight()
{
    return m_properties[FISH_LINE_WEIGHT].toString();
}

QString Fish::getReleaseWidth()
{
    return m_properties[FISH_RELEASE_WIDTH].toString();
}

QString Fish::getSpecies()
{
    return m_properties[FISH_SPECIES].toString();
}

QString Fish::getMethod()
{
    return m_properties[FISH_METHOD].toString();
}

QString Fish::getGetter()
{
    return m_properties[FISH_GETTER].toString();
}

QString Fish::getMiscText()
{
    return m_properties[FISH_MISC_TEXT].toString();
}

QString Fish::getWaterTemp()
{
    return m_properties[FISH_WATER_TEMP].toString();
}

QString Fish::getAirTemp()
{
    return m_properties[FISH_AIR_TEMP].toString();
}

QString Fish::getTrollingSpeedEstimate()
{
    if(QSettings().value("use_routelog").toBool())
    {
        QDateTime time(m_parent->getDate(), getTime());
        RouteInfo info(m_parent);
        double speed = info.speedAt(time);
        if(speed > 0)
        {
            return QString::number(speed, 'f', 1);
        }
    }

    return QString();
}

QString Fish::getCoordinatesLat()
{
    if(m_properties[FISH_COORDINATES_LAT].toString().isEmpty())
    {
        if(QSettings().value("use_routelog").toBool())
        {
            RouteInfo info(m_parent);
            QDateTime datetime(m_parent->getDate(), getTime());
            TrackPoint pt = info.nearestPoint(datetime);
            if(pt.lat != 0)
            {
                return QString::number(pt.lat);
            }
        }
    }
    return m_properties[FISH_COORDINATES_LAT].toString();
}

QString Fish::getCoordinatesLon()
{
    if(m_properties[FISH_COORDINATES_LON].toString().isEmpty())
    {
        if(QSettings().value("use_routelog").toBool())
        {
            RouteInfo info(m_parent);
            QDateTime datetime(m_parent->getDate(), getTime());
            TrackPoint pt = info.nearestPoint(datetime);
            if(pt.lon != 0)
            {
                return QString::number(pt.lon);
            }
        }
    }

    return m_properties[FISH_COORDINATES_LON].toString();
}

QTime Fish::getTime() const
{
   return m_properties[FISH_TIME].toTime();
}

bool Fish::isGroup()
{
    return m_properties[FISH_IS_GROUP].toBool() == true;
}

int Fish::getGroupAmount()
{
    return m_properties[FISH_GROUP_AMOUNT].toInt();
}

bool Fish::isUnderSize()
{
       return m_properties[FISH_IS_UNDERSIZE].toBool() == true;
}

bool Fish::isCR()
{
       return m_properties[FISH_IS_CATCHRELEASED].toBool() == true;
}

int Fish::getWindCondition()
{
    return m_properties[FISH_WIND].toInt();
}

int Fish::getWeatherCondition()
{
    return m_properties[FISH_WEATHER].toInt();
}

int Fish::getPressureCondition()
{
    return m_properties[FISH_PRESSURE].toInt();
}

int Fish::getRainCondition()
{
    return m_properties[FISH_RAIN].toInt();
}

Fish::EWindDirection Fish::getWindDirection()
{
   return static_cast<Fish::EWindDirection>(m_properties[FISH_WIND_DIRECTION].toInt());
}

Fish::EPressureChange Fish::getPressureChange()
{
    return static_cast<Fish::EPressureChange>(m_properties[FISH_PRESSURE_CHANGE].toInt());
}
