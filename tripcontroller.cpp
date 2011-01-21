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
    if(!m_trip)
    {
        qCritical() << "Impossible to handle boolean event. No trip!";
        return QDate::currentDate();
    }

    switch(source)
    {
    case eTripDate: return m_trip->getDate(); break;
    }
}

bool TripController::getBooleanValue(EUISource source)
{
    if(!m_trip)
    {
        qCritical() << "Impossible to handle boolean event. No trip!";
        return false;
    }

    static bool bPrev = true;
    if(bPrev)
        bPrev = false;
    else
        bPrev = true;
    switch(source)
    {
    case eTime49: return (m_trip->getTimeMask()&1)!=0; break;
    case eTime911: return (m_trip->getTimeMask()&2)!=0; break;
    case eTime1114: return (m_trip->getTimeMask()&4)!=0; break;
    case eTime1418: return (m_trip->getTimeMask()&8)!=0; break;
    case eTime1823: return (m_trip->getTimeMask()&16)!=0; break;
    case eTime2304: return (m_trip->getTimeMask()&32)!=0; break;
    }
}

void TripController::booleanEvent(EUISource source, bool value)
{
    if(!m_trip)
    {
        qCritical() << "Impossible to handle boolean event. No trip!";
        return;
    }

    switch(source)
    {
    case eTime49:
    case eTime911:
    case eTime1114:
    case eTime1418:
    case eTime1823:
    case eTime2304: m_trip->setTimeMask(pow(2,source-eTime49),value);
        break;

    case eWindCalm:
    case eWindFaint:
    case eWindModerate:
    case eWindBrisk:
    case eWindHard: qDebug() << "wind" << source-eWindCalm;

        break;

    case eWeatherClear:
    case eWeatherHalfClear:
    case eWeatherOvercast:
    case eWeatherRain:
    case eWeatherFog: qDebug() << "weather" << source-eWeatherClear;
        break;

    case eUnderSize: break;
    default: qCritical() << "Unknown boolean event. Cant handle this!" << source;
    }

    sendNotificationToObservers();
}

void TripController::intEvent(EUISource source, int value)
{
    if(m_trip == NULL)
    {
        qCritical() << "Impossible to handle int event. No trip!";
        return;
    }

    switch(source)
    {
    case eSpecies: break;
    case eMethod: break;
    default:  qCritical() << "Unknown int event. Cant handle this!" << source;
    }
}

void TripController::dateEvent(EUISource source, const QDate& value)
{
    if(m_trip == NULL)
    {
        qCritical() << "Impossible to handle date event. No trip!";
        return;
    }

    switch(source)
    {
    case eTripDate: m_trip->setDate(value); break;
    default:  qCritical() << "Unknown date event. Cant handle this!" << source;
    }
    sendNotificationToObservers();
}

void TripController::textEvent(EUISource source, const QString& value)
{
    if(!m_trip)
    {
        qCritical() << "Impossible to handle text event. No trip!";
        return;
    }

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
    sendNotificationToObservers();
}

void TripController::buttonEvent(EUISource source)
{
    switch(source)
    {
    case eSaveTrip: {
        int newId = Singletons::model()->commit(m_trip);
        m_trip = Singletons::model()->getTrip(newId);
        } break;
    default:  qCritical() << "Unknown default event. Cant handle this!" << source;
    }
    sendNotificationToObservers();
}

