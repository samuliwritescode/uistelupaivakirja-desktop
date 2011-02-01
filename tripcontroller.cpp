#include <QDebug>
#include <math.h>
#include "singletons.h"
#include "tripcontroller.h"
#include "trollingmodel.h"
#include "locationprovider.h"

TripController::TripController(QObject *parent) :
        Controller(parent),
        m_trip(NULL)
{
    m_trip = Singletons::model()->getTrip();
}

QDate TripController::getDateValue(EUISource source)
{
    switch(source)
    {
    case eTripDate: return m_trip->getDate(); break;
    default: qCritical() << "Unknown get date" << source; break;
    }
    return QDate();
}

bool TripController::getBooleanValue(EUISource source)
{
    switch(source)
    {
    case eWindCalm: return m_trip->getFish()->isProperty(FISH_WIND, Fish::eCalm); break;
    case eWindFaint: return m_trip->getFish()->isProperty(FISH_WIND, Fish::eFaint); break;
    case eWindModerate: return m_trip->getFish()->isProperty(FISH_WIND, Fish::eModerate); break;
    case eWindBrisk: return m_trip->getFish()->isProperty(FISH_WIND, Fish::eBrisk); break;
    case eWindHard: return m_trip->getFish()->isProperty(FISH_WIND, Fish::eHard); break;

    case eWeatherClear: return m_trip->getFish()->isProperty(FISH_WEATHER, Fish::eClear); break;
    case eWeatherHalfClear: return m_trip->getFish()->isProperty(FISH_WEATHER, Fish::eHalfClear); break;
    case eWeatherOvercast: return m_trip->getFish()->isProperty(FISH_WEATHER, Fish::eOvercast); break;
    case eWeatherRain: return m_trip->getFish()->isProperty(FISH_WEATHER, Fish::eRain); break;
    case eWeatherFog: return m_trip->getFish()->isProperty(FISH_WEATHER, Fish::eFog); break;

    case ePressureLow: return m_trip->getFish()->isProperty(FISH_PRESSURE, Fish::eLow); break;
    case ePressureMildLow: return m_trip->getFish()->isProperty(FISH_PRESSURE, Fish::eMildLow); break;
    case ePressureNormal: return m_trip->getFish()->isProperty(FISH_PRESSURE, Fish::eNormal); break;
    case ePressureMildHigh: return m_trip->getFish()->isProperty(FISH_PRESSURE, Fish::eMildHigh); break;
    case ePressureHigh: return m_trip->getFish()->isProperty(FISH_PRESSURE, Fish::eHigh); break;

    case eGroup: return m_trip->getFish()->isProperty(FISH_IS_GROUP, true); break;
    case eCatchNRelease: return m_trip->getFish()->isProperty(FISH_IS_CATCHRELEASED, true); break;
    case eUnderSize: return m_trip->getFish()->isProperty(FISH_IS_UNDERSIZE, true); break;
    case eUnsavedChanges: return m_trip->isUnsaved(); break;
    default: qCritical() << "Unknown get boolean" << source; break;
    }
    return false;
}

int TripController::getIntValue(EUISource source)
{
    switch(source)
    {
    case eFishList: return m_trip->getSelectedFish(); break;
    case eFishType: return m_trip->getFish()->getType(); break;
    case eStartTime: return m_trip->getTime().first.hour(); break;
    case eEndTime: return m_trip->getTime().second.hour(); break;
    case eWindDirection: return m_trip->getFish()->getProperty(FISH_WIND_DIRECTION).toInt(); break;
    case ePressureChange: return m_trip->getFish()->getProperty(FISH_PRESSURE_CHANGE).toInt(); break;
    case ePlaceName:
        if(m_trip->getPlace())
            return m_trip->getPlace()->getId();
        else
            return -1;
        break;
    default: qCritical() << "Unknown get int" << source; break;
    }
    return 0;
}

QString TripController::getTextValue(EUISource source)
{
    switch(source)
    {
    case eLureName:
        if(m_trip->getFish()->getLure())
            return m_trip->getFish()->getLure()->getMaker();
        break;
    case eLength: return m_trip->getFish()->getProperty(FISH_LENGTH).toString(); break;
    case eWeight: return m_trip->getFish()->getProperty(FISH_WEIGHT).toString(); break;
    case eSpotDepth: return m_trip->getFish()->getProperty(FISH_SPOT_DEPTH).toString(); break;
    case eTotalDepth: return m_trip->getFish()->getProperty(FISH_TOTAL_DEPTH).toString(); break;
    case eTrollingSpeed: return m_trip->getFish()->getProperty(FISH_TROLLING_SPEED).toString(); break;
    case eLineWeight: return m_trip->getFish()->getProperty(FISH_LINE_WEIGHT).toString(); break;
    case eReleaseWidth: return m_trip->getFish()->getProperty(FISH_RELEASE_WIDTH).toString(); break;
    case eAirTemp: return m_trip->getFish()->getProperty(FISH_AIR_TEMP).toString(); break;
    case eWaterTemp: return m_trip->getFish()->getProperty(FISH_WATER_TEMP).toString(); break;
    case eMiscText: return m_trip->getFish()->getProperty(FISH_MISC_TEXT).toString(); break;
    case eSpecies: return m_trip->getFish()->getProperty(FISH_SPECIES).toString(); break;
    case eMethod: return m_trip->getFish()->getProperty(FISH_METHOD).toString(); break;
    case eGetter: return m_trip->getFish()->getProperty(FISH_GETTER).toString(); break;
    case eWayPointSet: return m_trip->getFish()->getProperty(FISH_COORDINATES_LAT).toString(); break;
    default: qCritical() << "Unknown get text" << source; break;
    }
    return QString();
}

QTime TripController::getTimeValue(EUISource source)
{
    switch(source)
    {
    case eTime:
        if(m_trip->getFish()->getProperty(FISH_TIME).toTime().isValid())
            return m_trip->getFish()->getProperty(FISH_TIME).toTime();
        else
            return QTime(0,0,0);

        break;
        default: qCritical() << "unknown get time" << source;
    }
    return QTime();
}

void TripController::booleanEvent(EUISource source, bool value)
{
    qDebug() << "got bool event" << source << value;
    switch(source)
    {
    case eWindCalm: m_trip->getFish()->setProperty(FISH_WIND, Fish::eCalm); break;
    case eWindFaint: m_trip->getFish()->setProperty(FISH_WIND, Fish::eFaint); break;
    case eWindModerate: m_trip->getFish()->setProperty(FISH_WIND, Fish::eModerate); break;
    case eWindBrisk: m_trip->getFish()->setProperty(FISH_WIND, Fish::eBrisk); break;
    case eWindHard: m_trip->getFish()->setProperty(FISH_WIND, Fish::eHard); break;

    case eWeatherClear: m_trip->getFish()->setProperty(FISH_WEATHER, Fish::eClear); break;
    case eWeatherHalfClear: m_trip->getFish()->setProperty(FISH_WEATHER, Fish::eHalfClear); break;
    case eWeatherOvercast: m_trip->getFish()->setProperty(FISH_WEATHER, Fish::eOvercast); break;
    case eWeatherRain: m_trip->getFish()->setProperty(FISH_WEATHER, Fish::eRain); break;
    case eWeatherFog: m_trip->getFish()->setProperty(FISH_WEATHER, Fish::eFog); break;

    case ePressureLow: m_trip->getFish()->setProperty(FISH_PRESSURE, Fish::eLow); break;
    case ePressureMildLow: m_trip->getFish()->setProperty(FISH_PRESSURE, Fish::eMildLow); break;
    case ePressureNormal: m_trip->getFish()->setProperty(FISH_PRESSURE, Fish::eNormal); break;
    case ePressureMildHigh: m_trip->getFish()->setProperty(FISH_PRESSURE, Fish::eMildHigh); break;
    case ePressureHigh: m_trip->getFish()->setProperty(FISH_PRESSURE, Fish::eHigh); break;

    case eGroup: m_trip->getFish()->setProperty(FISH_IS_GROUP, value); break;
    case eCatchNRelease: m_trip->getFish()->setProperty(FISH_IS_CATCHRELEASED, value); break;
    case eUnderSize: m_trip->getFish()->setProperty(FISH_IS_UNDERSIZE, value); break;
    default: qCritical() << "Unknown boolean event. Cant handle this!" << source << value;
    }

    sendNotificationToObservers(Controller::eTripUpdated);
}

void TripController::intEvent(EUISource source, int value)
{
    qDebug() << "got int event" << source << value;
    switch(source)
    {
    case eStartTime: m_trip->setTime(QTime(value,0), QTime()); break;
    case eEndTime: m_trip->setTime(QTime(), QTime(value,0)); break;
    case eWindDirection:
        m_trip->getFish()->setProperty(FISH_WIND_DIRECTION, value);
        sendNotificationToObservers(Controller::eTripUpdated);
        return; break;
    case ePressureChange:
        m_trip->getFish()->setProperty(FISH_PRESSURE_CHANGE, value);
        sendNotificationToObservers(Controller::eTripUpdated);
        return; break;
    case eTrip:
        m_trip = Singletons::model()->getTrip(value);
        m_trip->selectFish(-1);
        sendNotificationToObservers(Controller::eFishListUpdated);
        sendNotificationToObservers(Controller::eWayPointsUpdated);
        break;
    case eFishList:
        m_trip->selectFish(value);
        sendNotificationToObservers(Controller::eTripUpdated);
        return;
        break;
    case ePlaceName:
        if(Singletons::model()->getPlace(value))
        {
            m_trip->setPlace(Singletons::model()->getPlace(value));
        }
        sendNotificationToObservers(Controller::eTripUpdated);
        return;
        break;
    case eSelectLure:

        if(Singletons::model()->getLure(value))
        {
            m_trip->getFish()->setLure(Singletons::model()->getLure(value));
        }

        sendNotificationToObservers(Controller::eTripUpdated);
        return;
        break;
    case eWayPointSet:
        {
            WayPoint wpt = m_trip->getWayPoints().at(value);
            m_trip->getFish()->setProperty(FISH_COORDINATES_LAT, QString::number(wpt.lat));
            m_trip->getFish()->setProperty(FISH_COORDINATES_LON, QString::number(wpt.lon));
            m_trip->getFish()->setProperty(FISH_TIME, wpt.time);
        }
        break;
    default:  qCritical() << "Unknown int event. Cant handle this!" << source;
    }
    sendNotificationToObservers(Controller::eTripUpdated);    
    sendNotificationToObservers(Controller::eFishListUpdated);
    sendNotificationToObservers(Controller::eFishPropertyUpdated);
}

void TripController::dateEvent(EUISource source, const QDate& value)
{
    qDebug() << "got date event" << source << value;
    switch(source)
    {
    case eTripDate: m_trip->setDate(value); break;
    default:  qCritical() << "Unknown date event. Cant handle this!" << source;
    }
    sendNotificationToObservers(Controller::eTripUpdated);
}

void TripController::timeEvent(EUISource source, const QTime& value)
{
    qDebug() << "got time event" << source << value;
    switch(source)
    {
    case eTime: m_trip->getFish()->setProperty(FISH_TIME, value); break;
    default:  qCritical() << "Unknown time event. Cant handle this!" << source;
    }
    sendNotificationToObservers(Controller::eTripUpdated);
    sendNotificationToObservers(Controller::eFishPropertyUpdated);
}

void TripController::textEvent(EUISource source, const QString& value)
{
    qDebug() << "got text event" << source << value;
    switch(source)
    {
    case eMiscText: m_trip->getFish()->setProperty(FISH_MISC_TEXT, value); break;
    case eWaterTemp: m_trip->getFish()->setProperty(FISH_WATER_TEMP, value); break;
    case eAirTemp: m_trip->getFish()->setProperty(FISH_AIR_TEMP, value); break;
    case eWeight: m_trip->getFish()->setProperty(FISH_WEIGHT, value); break;
    case eLength: m_trip->getFish()->setProperty(FISH_LENGTH, value); break;
    case eSpotDepth: m_trip->getFish()->setProperty(FISH_SPOT_DEPTH, value); break;
    case eTotalDepth: m_trip->getFish()->setProperty(FISH_TOTAL_DEPTH, value); break;
    case eTrollingSpeed:m_trip->getFish()->setProperty(FISH_TROLLING_SPEED, value); break;
    case eLineWeight: m_trip->getFish()->setProperty(FISH_LINE_WEIGHT, value); break;
    case eReleaseWidth: m_trip->getFish()->setProperty(FISH_RELEASE_WIDTH, value); break;
    case eSpecies: m_trip->getFish()->setProperty(FISH_SPECIES, value); break;
    case eMethod: m_trip->getFish()->setProperty(FISH_METHOD, value); break;
    case eGetter: m_trip->getFish()->setProperty(FISH_GETTER, value); break;
    case eWaypointsAdd: m_trip->setWayPoints(value);
        sendNotificationToObservers(Controller::eWayPointsUpdated); ;break;
    default:  qCritical() << "Unknown text event. Cant handle this!" << source;
    }

    sendNotificationToObservers(Controller::eFishPropertyUpdated);
}

void TripController::buttonEvent(EUISource source)
{
    qDebug() << "got button event" << source;
    switch(source)
    {
    case eSaveTrip: Singletons::model()->commit(m_trip); break;
    case eNewTrip: m_trip = Singletons::model()->getTrip(); break;
    case eDeleteTrip:
        Singletons::model()->remove(m_trip);
        m_trip = Singletons::model()->getTrip();
        break;
    case eNewFish:
        m_trip->newFish(Fish::eFish);
        sendNotificationToObservers(Controller::eFishListUpdated);
        sendNotificationToObservers(Controller::eTripUpdated);
        return;
        break;
    case eNewWeather:
        m_trip->newFish(Fish::eWeather);
        sendNotificationToObservers(Controller::eFishListUpdated);
        sendNotificationToObservers(Controller::eTripUpdated);
        return;
        break;
    case eNewFishWeather:
        m_trip->newFish(Fish::eFishAndWeather);
        sendNotificationToObservers(Controller::eFishListUpdated);
        sendNotificationToObservers(Controller::eTripUpdated);
        return;
        break;
    case eDeleteFish:
        m_trip->deleteFish(m_trip->getSelectedFish());
        sendNotificationToObservers(Controller::eFishListUpdated);
        sendNotificationToObservers(Controller::eTripUpdated);
        return;
        break;
    default:  qCritical() << "Unknown button event. Cant handle this!" << source;
    }
    sendNotificationToObservers(Controller::eTripUpdated);
    sendNotificationToObservers(Controller::eTripListUpdated);
    sendNotificationToObservers(Controller::eFishListUpdated);
}

QMap<QString, int> TripController::getTripList()
{
    QMap<QString, int> retval;
    QMap<int, Trip*> trips = Singletons::model()->getTrips();

    for(QMap<int, Trip*>::iterator iter = trips.begin(); iter!=trips.end(); iter++)
    {
        Trip* trip = iter.value();
        QString name = trip->getDate().toString("yyyy-MM-dd");
        name += " ";
        name += trip->getTime().first.toString("HH");
        name += "-";
        name +=trip->getTime().second.toString("HH");

        if(trip->getPlace())
        {
            name += ", ";
            name += trip->getPlace()->getName();
        }
        while(retval.contains(name))
        {
            name += "-1";
        }

        if(trip->getId() < 0)
            name = tr("tallentamaton uusi reissu");

        retval[name] = trip->getId();        
    }
    return retval;
}

QList<QMap<QString, QString> > TripController::getFishList()
{
    QList<QMap<QString, QString> > retval;
    for(int loop=0; loop < m_trip->getFishCount(); loop++)
    {
        QMap<QString, QString> props;
        Fish* fish = m_trip->getFish(loop);
        QList<QString> propnames = fish->getPropertyNames();
        foreach(QString propname, propnames)
        {
            props[propname] = fish->getProperty(propname).toString();
        }

        if(fish->getLure())
            props["lure"] = fish->getLure()->getMaker();

        retval.push_back(props);
    }
    return retval;
}

QList<QPair<QString, int> > TripController::getWayPointsList()
{
    QList<QPair<QString, int> > retval;
    QList<WayPoint> waypoints = m_trip->getWayPoints();
    for(int loop=0; loop < waypoints.count(); loop++)
    {
        WayPoint wpt=waypoints.at(loop);
        QPair<QString, int> pair;
        pair.first = wpt.name;
        pair.second = loop;
        retval.push_back(pair);
    }
    return retval;
}

QStringList TripController::getAlternatives(EUISource source)
{
    QStringList retval;
    QString key;
    switch(source)
    {
    case eSpecies: key = FISH_SPECIES; break;
    case eGetter: key = FISH_GETTER; break;
    case eMethod: key = FISH_METHOD; break;
    default: break;
    }

    QMap<int, Trip*> trips = Singletons::model()->getTrips();
    foreach(Trip* trip, trips)
    {
        for(int loop=0; loop < trip->getFishCount(); loop++)
        {
            Fish* fish = trip->getFish(loop);
            if(!retval.contains(fish->getProperty(key).toString()))
            {
                retval.append(fish->getProperty(key).toString());
            }
        }
    }

    return retval;
}

