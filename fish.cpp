#include <QDebug>
#include "fish.h"
#include "trip.h"
#include "trollingexception.h"

Fish::Fish():
        m_lure(NULL)
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
    m_lure = p_lure;
}


Lure* Fish::getLure()
{
    return m_lure;
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
    EWeatherCondition weather = getWeatherCondition();
    switch(weather)
    {
    case eClear: return QObject::tr("Selkeää");
    case eHalfClear: return QObject::tr("Puolipilvistä");
    case eOvercast: return QObject::tr("Pilvistä");
    case eRain: return QObject::tr("Sadetta");
    case eFog: return QObject::tr("Sumua");
    default: break;
    }
    return QObject::tr("n/a");
}

QString Fish::getHumanReadableWind()
{
    EWindCondition wind = getWindCondition();
    switch(wind)
    {
    case eCalm: return QObject::tr("Tyyntä");
    case eFaint: return QObject::tr("Heikkoa");
    case eModerate: return QObject::tr("Kohtalaista");
    case eBrisk: return QObject::tr("Navakkaa");
    case eHard: return QObject::tr("Kovaa");
    default: break;
    }
    return QObject::tr("n/a");
}

QString Fish::getHumanReadablePressure()
{
    EPressureCondition pressure = getPressureCondition();
    switch(pressure)
    {
    case eLow: return QObject::tr("Matala");
    case eMildLow: return QObject::tr("Melko matala");
    case eNormal: return QObject::tr("Normaali");
    case eMildHigh: return QObject::tr("Melko korkea");
    case eHigh: return QObject::tr("Korkea");
    default: break;
    }
    return QObject::tr("n/a");
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
        p_value.toDouble(&bConvOk);
        if(bConvOk)
        {
            setProperty(p_prop, p_value);
        }
        else
        {
            throw TrollingException("Value is not numeric: "+p_value+" property: "+p_prop);
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
    setDouble(FISH_TROLLING_SPEED, p_val);
}

void Fish::setLineWeight(const QString& p_val)
{
    setDouble(FISH_LINE_WEIGHT, p_val);
}

void Fish::setReleaseWidth(const QString& p_val)
{
    setDouble(FISH_RELEASE_WIDTH, p_val);
}

void Fish::setSpecies(const QString& p_val)
{
    setProperty(FISH_SPECIES, p_val);
}

void Fish::setMethod(const QString& p_val)
{
    setProperty(FISH_METHOD, p_val);
}

void Fish::setGetter(const QString& p_val)
{
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

void Fish::setWindCondition(EWindCondition p_val)
{
    setProperty(FISH_WIND, p_val);
}

void Fish::setWindDirection(EWindDirection p_val)
{
    setProperty(FISH_WIND_DIRECTION, p_val);
}

void Fish::setWeatherCondition(EWeatherCondition p_val)
{
    setProperty(FISH_WEATHER, p_val);
}

void Fish::setPressureCondition(EPressureCondition p_val)
{
    setProperty(FISH_PRESSURE, p_val);
}

void Fish::setPressureChange(EPressureChange p_val)
{
    setProperty(FISH_PRESSURE_CHANGE, p_val);
}

void Fish::setGroup(bool p_val)
{
    setProperty(FISH_IS_GROUP, p_val);
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

QString Fish::getCoordinatesLat()
{
    return m_properties[FISH_COORDINATES_LAT].toString();
}

QString Fish::getCoordinatesLon()
{
    return m_properties[FISH_COORDINATES_LON].toString();
}

QTime Fish::getTime()
{
   return m_properties[FISH_TIME].toTime();
}

bool Fish::isGroup()
{
    return m_properties[FISH_IS_GROUP].toBool() == true;
}

bool Fish::isUnderSize()
{
       return m_properties[FISH_IS_UNDERSIZE].toBool() == true;
}

bool Fish::isCR()
{
       return m_properties[FISH_IS_CATCHRELEASED].toBool() == true;
}

Fish::EWindCondition Fish::getWindCondition()
{
    return static_cast<Fish::EWindCondition>(m_properties[FISH_WIND].toInt());
}

Fish::EWeatherCondition Fish::getWeatherCondition()
{
    return static_cast<Fish::EWeatherCondition>(m_properties[FISH_WEATHER].toInt());
}

Fish::EPressureCondition Fish::getPressureCondition()
{
    return static_cast<Fish::EPressureCondition>(m_properties[FISH_PRESSURE].toInt());
}

Fish::EWindDirection Fish::getWindDirection()
{
   return static_cast<Fish::EWindDirection>(m_properties[FISH_WIND_DIRECTION].toInt());
}

Fish::EPressureChange Fish::getPressureChange()
{
    return static_cast<Fish::EPressureChange>(m_properties[FISH_PRESSURE_CHANGE].toInt());
}
