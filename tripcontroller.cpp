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
    //m_trip = Singletons::model()->getTrip();
}

QDate TripController::getDateValue(EUISource source)
{
    if(!m_trip) return QDate();

    switch(source)
    {
    case eTripDate: return m_trip->getDate(); break;
    default: qCritical() << "Unknown get date" << source; break;
    }
    return QDate();
}

bool TripController::getBooleanValue(EUISource source)
{
    if(!m_trip) return false;

    switch(source)
    {
    case eWindCalm: return m_trip->getFish()->getWindCondition() == Fish::eCalm; break;
    case eWindFaint: return m_trip->getFish()->getWindCondition() == Fish::eFaint; break;
    case eWindModerate: return m_trip->getFish()->getWindCondition() == Fish::eModerate; break;
    case eWindBrisk: return m_trip->getFish()->getWindCondition() == Fish::eBrisk; break;
    case eWindHard: return m_trip->getFish()->getWindCondition() == Fish::eHard; break;

    case eWeatherClear: return m_trip->getFish()->getWeatherCondition() == Fish::eClear; break;
    case eWeatherHalfClear: return m_trip->getFish()->getWeatherCondition() == Fish::eHalfClear; break;
    case eWeatherOvercast: return m_trip->getFish()->getWeatherCondition() == Fish::eOvercast; break;
    case eWeatherRain: return m_trip->getFish()->getWeatherCondition() == Fish::eRain; break;
    case eWeatherFog: return m_trip->getFish()->getWeatherCondition() == Fish::eFog; break;

    case ePressureLow: return m_trip->getFish()->getPressureCondition() == Fish::eLow; break;
    case ePressureMildLow: return m_trip->getFish()->getPressureCondition() == Fish::eMildLow; break;
    case ePressureNormal: return m_trip->getFish()->getPressureCondition() == Fish::eNormal; break;
    case ePressureMildHigh: return m_trip->getFish()->getPressureCondition() == Fish::eMildHigh; break;
    case ePressureHigh: return m_trip->getFish()->getPressureCondition() == Fish::eHigh; break;

    case eGroup: return m_trip->getFish()->isGroup(); break;
    case eCatchNRelease: return m_trip->getFish()->isCR(); break;
    case eUnderSize: return m_trip->getFish()->isUnderSize(); break;
    case eUnsavedChanges: return m_trip->isUnsaved(); break;
    case eTrip: return m_trip != NULL; break;
    default: qCritical() << "Unknown get boolean" << source; break;
    }
    return false;
}

int TripController::getIntValue(EUISource source)
{
    if(!m_trip) return 0;

    switch(source)
    {
    case eFishList: return m_trip->getSelectedFish(); break;
    case eFishType: return m_trip->getFish()->getType(); break;
    case eStartTime: return m_trip->getTime().first.hour(); break;
    case eEndTime: return m_trip->getTime().second.hour(); break;
    case eWindDirection: return m_trip->getFish()->getWindDirection(); break;
    case ePressureChange: return m_trip->getFish()->getPressureChange(); break;
    case ePlaceName:
        if(m_trip->getPlace())
            return m_trip->getPlace()->getId();
        else
            return -1;
        break;
    case eTrip: return m_trip->getId(); break;
    default: qCritical() << "Unknown get int" << source; break;
    }
    return 0;
}

QString TripController::getTextValue(EUISource source)
{
    if(!m_trip) return QString();

    switch(source)
    {
    case eLureName:
        if(m_trip->getFish()->getLure())
            return m_trip->getFish()->getLure()->getMaker();
        break;
    case eLength: return m_trip->getFish()->getLength();
    case eWeight: return m_trip->getFish()->getWeight();
    case eSpotDepth: return m_trip->getFish()->getSpotDepth();
    case eTotalDepth: return m_trip->getFish()->getTotalDepth();
    case eTrollingSpeed: return m_trip->getFish()->getTrollingSpeed();
    case eLineWeight: return m_trip->getFish()->getLineWeight();
    case eReleaseWidth: return m_trip->getFish()->getReleaseWidth();
    case eAirTemp: return m_trip->getFish()->getAirTemp();
    case eWaterTemp: return m_trip->getFish()->getWaterTemp();
    case eMiscText: return m_trip->getFish()->getMiscText();
    case eSpecies: return m_trip->getFish()->getSpecies();
    case eMethod: return m_trip->getFish()->getMethod();
    case eGetter: return m_trip->getFish()->getGetter();
    case eWayPointSet: return m_trip->getFish()->getCoordinatesLat()+" "+
                m_trip->getFish()->getCoordinatesLon();
    case eWindDirection: return m_trip->getFish()->getHumanReadableWindDirection();
    case ePressureChange: return m_trip->getFish()->getHumanReadablePressureChange();
    default: qCritical() << "Unknown get text" << source; break;
    }
    return QString();
}

QTime TripController::getTimeValue(EUISource source)
{
    if(!m_trip) return QTime();
    switch(source)
    {
    case eTime:
        if(m_trip->getFish()->getTime().isValid())
            return m_trip->getFish()->getTime();
        else
            return QTime(0,0,0);

        break;
        default: qCritical() << "unknown get time" << source;
    }
    return QTime();
}

void TripController::booleanEvent(EUISource source, bool value)
{
    if(!m_trip) return;
    qDebug() << "got bool event" << source << value;
    switch(source)
    {
    case eWindCalm: m_trip->getFish()->setWindCondition(Fish::eCalm); break;
    case eWindFaint: m_trip->getFish()->setWindCondition(Fish::eFaint); break;
    case eWindModerate: m_trip->getFish()->setWindCondition(Fish::eModerate); break;
    case eWindBrisk: m_trip->getFish()->setWindCondition(Fish::eBrisk); break;
    case eWindHard: m_trip->getFish()->setWindCondition(Fish::eHard); break;

    case eWeatherClear: m_trip->getFish()->setWeatherCondition(Fish::eClear); break;
    case eWeatherHalfClear: m_trip->getFish()->setWeatherCondition(Fish::eHalfClear); break;
    case eWeatherOvercast: m_trip->getFish()->setWeatherCondition(Fish::eOvercast); break;
    case eWeatherRain: m_trip->getFish()->setWeatherCondition(Fish::eRain); break;
    case eWeatherFog: m_trip->getFish()->setWeatherCondition(Fish::eFog); break;

    case ePressureLow: m_trip->getFish()->setPressureCondition(Fish::eLow); break;
    case ePressureMildLow: m_trip->getFish()->setPressureCondition(Fish::eMildLow); break;
    case ePressureNormal: m_trip->getFish()->setPressureCondition(Fish::eNormal); break;
    case ePressureMildHigh: m_trip->getFish()->setPressureCondition(Fish::eMildHigh); break;
    case ePressureHigh: m_trip->getFish()->setPressureCondition(Fish::eHigh); break;

    case eGroup: m_trip->getFish()->setGroup(value); break;
    case eCatchNRelease: m_trip->getFish()->setCR(value); break;
    case eUnderSize: m_trip->getFish()->setUnderSize(value); break;
    default: qCritical() << "Unknown boolean event. Cant handle this!" << source << value;
    }

    sendNotificationToObservers(Controller::eTripUpdated);
}

void TripController::intEvent(EUISource source, int value)
{
    if(!m_trip && source != eTrip) return;
    qDebug() << "got int event" << source << value;
    switch(source)
    {
    case eStartTime: m_trip->setTime(QTime(value,0), QTime()); break;
    case eEndTime: m_trip->setTime(QTime(), QTime(value,0)); break;
    case eWindDirection:
        m_trip->getFish()->setWindDirection(static_cast<Fish::EWindDirection>(value));
        sendNotificationToObservers(Controller::eTripUpdated);
        return; break;
    case ePressureChange:
        m_trip->getFish()->setPressureChange(static_cast<Fish::EPressureChange>(value));
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
            m_trip->getFish()->setCoordinates(QString::number(wpt.lat), QString::number(wpt.lon));
            m_trip->getFish()->setTime(wpt.time.time());
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
    if(!m_trip) return;

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
    if(!m_trip) return;

    qDebug() << "got time event" << source << value;
    switch(source)
    {
    case eTime: m_trip->getFish()->setTime(value); break;
    default:  qCritical() << "Unknown time event. Cant handle this!" << source;
    }
    sendNotificationToObservers(Controller::eTripUpdated);
    sendNotificationToObservers(Controller::eFishPropertyUpdated);
}

void TripController::textEvent(EUISource source, const QString& value)
{
    if(!m_trip) return;

    qDebug() << "got text event" << source << value;
    switch(source)
    {
    case eMiscText: m_trip->getFish()->setMiscText(value); break;
    case eWaterTemp: m_trip->getFish()->setWaterTemp(value); break;
    case eAirTemp: m_trip->getFish()->setAirTemp(value); break;
    case eWeight: m_trip->getFish()->setWeight(value); break;
    case eLength: m_trip->getFish()->setLength(value); break;
    case eSpotDepth: m_trip->getFish()->setSpotDepth(value); break;
    case eTotalDepth: m_trip->getFish()->setTotalDepth(value); break;
    case eTrollingSpeed:m_trip->getFish()->setTrollingSpeed(value); break;
    case eLineWeight: m_trip->getFish()->setLineWeight(value); break;
    case eReleaseWidth: m_trip->getFish()->setReleaseWidth(value); break;
    case eSpecies: m_trip->getFish()->setSpecies(value); break;
    case eMethod: m_trip->getFish()->setMethod(value); break;
    case eGetter: m_trip->getFish()->setGetter(value); break;
    case eUserField: {
            if(value.split("\n").count() != 2)
                break;
            m_trip->getFish()->setUserField(value.split("\n").at(0),
                                            value.split("\n").at(1));
        }
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
    case eTripUndo:
        {
            int id = m_trip->getId();
            Singletons::model()->reset(m_trip);
            m_trip = Singletons::model()->getTrip(id);
        }break;
    case eSaveTrip: Singletons::model()->commit(m_trip); break;
    case eNewTrip: m_trip = Singletons::model()->getTrip(); break;
    case eDeleteTrip:
        Singletons::model()->remove(m_trip);
        m_trip = NULL;
        //m_trip = Singletons::model()->getTrip();
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
    if(!m_trip) return retval;

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

QList<QString> TripController::getUserFields()
{
    QList<QString> retval;
    Fish* fish = m_trip->getFish();
    QMap<QString, QString> userfields = fish->getUserFields();
    int index = 0;
    while(userfields.contains(QString::number(index)))
    {
        retval.push_back(userfields[QString::number(index)]);
        index++;
    }
    return retval;
}

