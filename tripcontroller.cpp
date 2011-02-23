#include <QDebug>
#include <math.h>
#include "singletons.h"
#include "tripcontroller.h"
#include "trollingmodel.h"
#include "locationprovider.h"
#include "trollingexception.h"

TripController::TripController(QObject *parent) :
        Controller(parent),
        m_trip(NULL)
{
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
    case eUnsavedChanges: return m_trip->isUnsaved(); break;
    case eTrip: return true; break;
    default: break;
    }

    if(!m_trip->getFish()) return false;

    switch(source)
    {
    case eGroup: return m_trip->getFish()->isGroup(); break;
    case eCatchNRelease: return m_trip->getFish()->isCR(); break;
    case eUnderSize: return m_trip->getFish()->isUnderSize(); break;
    default: break;
    }

    return false;
}

int TripController::getIntValue(EUISource source)
{
    if(!m_trip) return 0;

    switch(source)
    {
    case eFishList: return m_trip->getSelectedFish(); break;    
    case eStartTime: return m_trip->getTime().first.hour(); break;
    case eEndTime: return m_trip->getTime().second.hour(); break;
    case ePlaceName:
        if(m_trip->getPlace())
            return m_trip->getPlace()->getId();
        else
            return -1;
        break;
    case eTrip: return m_trip->getId(); break;
    default: break;
    }

    if(!m_trip->getFish()) return 0;

    switch(source)
    {
    case eFishType: return m_trip->getFish()->getType(); break;
    case eWindDirection: return m_trip->getFish()->getWindDirection(); break;
    case ePressureChange: return m_trip->getFish()->getPressureChange(); break;
    case eGroup: return m_trip->getFish()->getGroupAmount(); break;
    case eWeather: return m_trip->getFish()->getWeatherCondition(); break;
    case eRain: return m_trip->getFish()->getRainCondition(); break;
    case ePressure: return m_trip->getFish()->getPressureCondition(); break;
    case eWind: return m_trip->getFish()->getWindCondition(); break;
    default: break;
    }
    return 0;
}

QString TripController::getTextValue(EUISource source)
{
    if(!m_trip) return QString();

    switch(source)
    { 
    case eTripDescription: return m_trip->getDescription();
    default: break;
    }

    if(!m_trip->getFish()) return QString();

    switch(source)
    {
    case eLureName: return Singletons::lureController()->lureName(m_trip->getFish()->getLure()); break;
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
    case eWayPointSet:
        if(!m_trip->getFish()->getCoordinatesLat().isEmpty() &&
           !m_trip->getFish()->getCoordinatesLon().isEmpty())
        {
            return m_trip->getFish()->getCoordinatesLat()+" "+m_trip->getFish()->getCoordinatesLon();
        }
        break;
    case eWindDirection: return m_trip->getFish()->getHumanReadableWindDirection();
    case ePressureChange: return m_trip->getFish()->getHumanReadablePressureChange();
    case eWeather: return m_trip->getFish()->getHumanReadableWeather(); break;
    case eRain: return m_trip->getFish()->getHumanReadableRain(); break;
    case ePressure: return m_trip->getFish()->getHumanReadablePressure(); break;
    case eWind: return m_trip->getFish()->getHumanReadableWind(); break;
    default: break;
    }

    return QString();
}

QTime TripController::getTimeValue(EUISource source)
{
    if(!m_trip) return QTime();
    switch(source)
    {
    case eStartTime: return m_trip->getTime().first; break;
    case eEndTime: return m_trip->getTime().second; break;
    default: break;
    }

    if(!m_trip->getFish()) return QTime();

    switch(source)
    {
    case eTime:
        if(m_trip->getFish()->getTime().isValid())
            return m_trip->getFish()->getTime();
        else
            return QTime(0,0,0);
        break;
    default: break;
    }
    return QTime();
}

void TripController::booleanEvent(EUISource source, bool value)
{
    if(!m_trip) return;
    try
    {
        qDebug() << "got bool event" << source << value;
        switch(source)
        {
        case eTripReportShowImages: m_report.setShowImages(value); break;
        case eTripReportShowMaps: m_report.setShowMaps(value); break;
        default: break;
        }

        if(m_trip->getFish())
        {
            switch(source)
            {
            case eGroup: m_trip->getFish()->setGroup(value); break;
            case eCatchNRelease: m_trip->getFish()->setCR(value); break;
            case eUnderSize: m_trip->getFish()->setUnderSize(value); break;
            default: break;
            }
        }
    }
    catch(TrollingException e)
    {
        showErrorMessage(e.toString());
    }

    sendNotificationToObservers(Controller::eTripUpdated);
    sendNotificationToObservers(Controller::eFishListUpdated);
}

void TripController::selectTrip(int value)
{
    if(m_trip && m_trip->isUnsaved())
    {
        int choice = showChoiseMessage(tr("Nykyinen reissu on tallentamatta. Haluatko tallentaa muutokset?"));
        if(choice == MessageDisplay::eCancel)
        {
            sendNotificationToObservers(Controller::eTripListUpdated);
            return;
        }
        else if(choice == MessageDisplay::eYes)
        {
            Singletons::model()->commit(m_trip);
        }
        else
        {
            Singletons::model()->reset(m_trip);
        }

    }
    m_trip = Singletons::model()->getTrip(value);
    m_trip->selectFish(-1);
    sendNotificationToObservers(Controller::eFishListUpdated);
    sendNotificationToObservers(Controller::eWayPointsUpdated);
}

void TripController::intEvent(EUISource source, int value)
{
    if(!m_trip && source != eTrip) return;

    try
    {
        qDebug() << "got int event" << source << value;
        switch(source)
        {
            case eStartTime: m_trip->setTime(QTime(value,m_trip->getTime().first.minute()), QTime()); break;
            case eEndTime: m_trip->setTime(QTime(), QTime(value, m_trip->getTime().second.minute())); break;
            case eTrip:
                selectTrip(value);
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
            default:  break;
        }

        if(m_trip->getFish())
        {
            switch(source)
            {
                case eWindDirection:
                    m_trip->getFish()->setWindDirection(static_cast<Fish::EWindDirection>(value));
                    sendNotificationToObservers(Controller::eTripUpdated);
                    return; break;
                case ePressureChange:
                    m_trip->getFish()->setPressureChange(static_cast<Fish::EPressureChange>(value));
                    sendNotificationToObservers(Controller::eTripUpdated);
                    return; break;
                case eSelectLure:

                    if(Singletons::model()->getLure(value))
                    {
                        m_trip->getFish()->setLure(Singletons::model()->getLure(value));
                    }

                    sendNotificationToObservers(Controller::eTripUpdated);
                    sendNotificationToObservers(Controller::eFishPropertyUpdated);
                    return;
                    break;
                case eWayPointSet:
                    {
                        WayPoint wpt = m_trip->getWayPoints().at(value);
                        m_trip->getFish()->setCoordinates(QString::number(wpt.lat), QString::number(wpt.lon));
                        m_trip->getFish()->setTime(wpt.time.time());
                        m_trip->getFish()->setMiscText(wpt.name);
                    }
                    break;
                case eGroup:
                    m_trip->getFish()->setGroupAmount(value);
                    break;
                case eWeather:
                    m_trip->getFish()->setWeatherCondition(value);
                    break;
                case eWind:
                    m_trip->getFish()->setWindCondition(value);
                    break;
                case ePressure:
                    m_trip->getFish()->setPressureCondition(value);
                    break;
                case eRain:
                    m_trip->getFish()->setRainCondition(value);
                    break;
                default:  break;
            }
        }
    }
    catch(TrollingException e)
    {
        showErrorMessage(e.toString());
    }
    sendNotificationToObservers(Controller::eTripUpdated);    
    sendNotificationToObservers(Controller::eFishListUpdated);
    sendNotificationToObservers(Controller::eFishPropertyUpdated);
}

void TripController::dateEvent(EUISource source, const QDate& value)
{
    if(!m_trip) return;
    try
    {
        qDebug() << "got date event" << source << value;
        switch(source)
        {
        case eTripDate: m_trip->setDate(value); break;
        default:  qCritical() << "Unknown date event. Cant handle this!" << source;
        }
    }
    catch(TrollingException e)
    {
        showErrorMessage(e.toString());
    }
    sendNotificationToObservers(Controller::eTripUpdated);
}

void TripController::timeEvent(EUISource source, const QTime& value)
{
    if(!m_trip) return;

    try
    {
        qDebug() << "got time event" << source << value;
        switch(source)
        {
        case eStartTime: m_trip->setTime(value, QTime()); break;
        case eEndTime: m_trip->setTime(QTime(), value); break;
        default: break;
        }

        if(m_trip->getFish())
        {
            switch(source)
            {
            case eTime: m_trip->getFish()->setTime(value); break;
            default: break;
            }
        }
    }
    catch(TrollingException e)
    {
        showErrorMessage(e.toString());
    }
    sendNotificationToObservers(Controller::eTripUpdated);
    sendNotificationToObservers(Controller::eFishPropertyUpdated);
}

void TripController::textEvent(EUISource source, const QString& value)
{
    if(!m_trip) return;

    qDebug() << "got text event" << source << value;
    try
    {
        switch(source)
        {
        case eTripDescription: m_trip->setDescription(value); break;
        case eWaypointsAdd:
            m_trip->setWayPoints(value);
            sendNotificationToObservers(Controller::eWayPointsUpdated);break;
        case eMediaFileAddTrip:
            m_trip->addMediaFile(value);
            sendNotificationToObservers(Controller::eTripUpdated);
            break;
        case eMediaFileRemoveTrip:
            m_trip->removeMediaFile(value);
            sendNotificationToObservers(Controller::eTripUpdated);
            break;
        default: break;
        }

        if(m_trip->getFish())
        {
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
                break;
            case eMediaFileAdd:
                m_trip->getFish()->addMediaFile(value);
                sendNotificationToObservers(Controller::eTripUpdated);break;
            case eMediaFileRemove:
                m_trip->getFish()->removeMediaFile(value);
                sendNotificationToObservers(Controller::eTripUpdated);
                break;
            default: break;
            }
        }
    }
    catch(TrollingException e)
    {
        showErrorMessage(e.toString());
    }
    sendNotificationToObservers(Controller::eFishPropertyUpdated);
}

void TripController::buttonEvent(EUISource source)
{
    qDebug() << "got button event" << source;
    try
    {
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
    }
    catch(TrollingException e)
    {
        showErrorMessage(e.toString());
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
        props[FISH_TIME] = fish->getTime().toString("HH:mm");
        if(fish->getType() == Fish::eWeather)
            props[FISH_SPECIES] = tr("säähav.");
        else
            props[FISH_SPECIES] = fish->getSpecies();
        props[FISH_WEIGHT] = fish->getWeight();
        props[FISH_LENGTH] = fish->getLength();
        props[FISH_WEATHER] = fish->getHumanReadableWeather();
        props[FISH_WATER_TEMP] = fish->getWaterTemp();
        props[FISH_WIND] = fish->getHumanReadableWind();
        props[FISH_GETTER] = fish->getGetter();
        props[FISH_METHOD] = fish->getMethod();
        props[FISH_AIR_TEMP] = fish->getAirTemp();
        props[FISH_TYPE] = QString::number(fish->getType());        
        props["lure"] = Singletons::lureController()->lureName(fish->getLure());

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
        pair.first = wpt.time.toString("dd.MM HH:mm")+" "+wpt.name;
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

QMap<QString, QString> TripController::getUserFields()
{
    if(m_trip && m_trip->getFish())
    {
        return m_trip->getFish()->getUserFields();
    }

    return QMap<QString, QString>();
}

QStringList TripController::getMediaFiles()
{
    QStringList retval;
    if(!m_trip && !m_trip->getFish()) return retval;

    retval = m_trip->getFish()->getMediaFiles();

    return retval;
}

QStringList TripController::getMediaFilesTrip()
{
    QStringList retval;
    if(!m_trip) return retval;

    retval = m_trip->getMediaFiles();

    return retval;
}

QString TripController::getTripReport()
{
    return m_report.getReport(m_trip);
}
