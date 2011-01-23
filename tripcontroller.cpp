#include <QDebug>
#include <math.h>
#include "singletons.h"
#include "tripcontroller.h"
#include "trollingmodel.h"

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
    default: break;
    }
    return QDate();
}

bool TripController::getBooleanValue(EUISource source)
{
    switch(source)
    {
    case eWindCalm: return m_trip->isWindCondition(Trip::eCalm); break;
    case eWindFaint: return m_trip->isWindCondition(Trip::eFaint); break;
    case eWindModerate: return m_trip->isWindCondition(Trip::eModerate); break;
    case eWindBrisk: return m_trip->isWindCondition(Trip::eBrisk); break;
    case eWindHard: return m_trip->isWindCondition(Trip::eHard); break;
    case eWeatherClear: return m_trip->isWeatherCondition(Trip::eClear); break;
    case eWeatherHalfClear: return m_trip->isWeatherCondition(Trip::eHalfClear); break;
    case eWeatherOvercast: return m_trip->isWeatherCondition(Trip::eOvercast); break;
    case eWeatherRain: return m_trip->isWeatherCondition(Trip::eRain); break;
    case eWeatherFog: return m_trip->isWeatherCondition(Trip::eFog); break;

    default: break;
    }
    return false;
}

int TripController::getIntValue(EUISource source)
{
    switch(source)
    {
    case eStartTime: return m_trip->getTime().first.hour(); break;
    case eEndTime: return m_trip->getTime().second.hour(); break;
    default: qCritical() << "Unknown int event. Cant handle this!" << source; break;
    }
    return 0;
}

void TripController::booleanEvent(EUISource source, bool value)
{
    switch(source)
    {

    case eWindCalm: m_trip->addWindCondition(Trip::eCalm, value); break;
    case eWindFaint: m_trip->addWindCondition(Trip::eFaint, value); break;
    case eWindModerate: m_trip->addWindCondition(Trip::eModerate, value); break;
    case eWindBrisk: m_trip->addWindCondition(Trip::eBrisk, value); break;
    case eWindHard: m_trip->addWindCondition(Trip::eHard, value); break;

    case eWeatherClear: m_trip->addWeatherCondition(Trip::eClear, value); break;
    case eWeatherHalfClear: m_trip->addWeatherCondition(Trip::eHalfClear, value); break;
    case eWeatherOvercast: m_trip->addWeatherCondition(Trip::eOvercast, value); break;
    case eWeatherRain: m_trip->addWeatherCondition(Trip::eRain, value); break;
    case eWeatherFog: m_trip->addWeatherCondition(Trip::eFog, value); break;
        break;

    case eUnderSize: break;
    default: qCritical() << "Unknown boolean event. Cant handle this!" << source << value;
    }

    sendNotificationToObservers(Controller::eTripUpdated);
}

void TripController::intEvent(EUISource source, int value)
{
    switch(source)
    {
    case eStartTime: m_trip->setTime(QTime(value,0), QTime()); break;
    case eEndTime: m_trip->setTime(QTime(), QTime(value,0)); break;
    case eTrip: m_trip = Singletons::model()->getTrip(value); break;
    case eSpecies: break;
    case eMethod: break;
    default:  qCritical() << "Unknown int event. Cant handle this!" << source;
    }
    sendNotificationToObservers(Controller::eTripUpdated);
}

void TripController::dateEvent(EUISource source, const QDate& value)
{
    switch(source)
    {
    case eTripDate: m_trip->setDate(value); break;
    default:  qCritical() << "Unknown date event. Cant handle this!" << source;
    }
    sendNotificationToObservers(Controller::eTripUpdated);
}

void TripController::textEvent(EUISource source, const QString& value)
{
    switch(source)
    {
    case ePlaceText: break;
    case eMiscText: m_trip->setDescription(value); break;
    case eWaterTemp: m_trip->setWaterTemp(value.toDouble()); break;
    case eStartTemp: break;
    case eEndTemp: break;
    case eWeight: break;
    case eLength: break;
    case eSpotDepth: break;
    default:  qCritical() << "Unknown text event. Cant handle this!" << source;
    }
    sendNotificationToObservers(Controller::eTripUpdated);
}

void TripController::buttonEvent(EUISource source)
{
    switch(source)
    {
    case eSaveTrip: Singletons::model()->commit(m_trip); break;
    case eNewTrip: m_trip = Singletons::model()->getTrip(); break;
    default:  qCritical() << "Unknown default event. Cant handle this!" << source;
    }
    sendNotificationToObservers(Controller::eTripUpdated);
    sendNotificationToObservers(Controller::eTripListUpdated);
}

QMap<QString, int> TripController::getTripList()
{
    QMap<QString, int> retval;
    QMap<int, Trip*> trips = Singletons::model()->getTrips();

    for(QMap<int, Trip*>::iterator iter = trips.begin(); iter!=trips.end(); iter++)
    {
        Trip* trip = iter.value();
        QString name = trip->getDate().toString();
        while(retval.contains(name))
        {
            name += "-1";
        }

        retval[name] = trip->getId();
        qDebug() << "content" <<trip->getDate().toString() << trip->getId();
    }
    return retval;
}


