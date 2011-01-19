#include <QDebug>
#include "singletons.h"
#include "tripcontroller.h"
#include "trollingmodel.h"

TripController::TripController(QObject *parent) :
        Controller(parent),
        m_trip(NULL)
{
    m_trip = Singletons::model()->getTrip();
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
    case eTime2304: qDebug() << "time" << source-eTime49;
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
}

