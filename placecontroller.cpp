#include "placecontroller.h"
#include "singletons.h"

PlaceController::PlaceController():
        m_site(NULL)
{
    m_site = Singletons::model()->getSite();
}

void PlaceController::buttonEvent(EUISource source)
{
    switch(source)
    {
    case ePlaceNew:
        Singletons::model()->commit(m_site);
        m_site = Singletons::model()->getSite();
        break;
    case ePlaceDelete:
        Singletons::model()->remove(m_site);
        m_site = Singletons::model()->getSite();
        break;
    default: qCritical() << "Dont know how to handle button event" << source; break;
    }

    sendNotificationToObservers(Controller::ePlaceListUpdated);
}

void PlaceController::booleanEvent(EUISource source, bool value)
{
    switch(source)
    {
    case ePlaceInvisible: m_site->setInvisible(value); break;
    default: qCritical() << "Dont know how to handle boolean event" << source; break;
    }
    sendNotificationToObservers(Controller::ePlaceUpdated);
}

void PlaceController::dateEvent(EUISource source, const QDate& value)
{
    qCritical() << "Dont know how to handle date event" << source;
}

void PlaceController::timeEvent(EUISource source, const QTime& value)
{
    qCritical() << "Dont know how to handle time event" << source;
}

void PlaceController::textEvent(EUISource source, const QString& value)
{

    switch(source)
    {
    case ePlaceName: m_site->setName(value); break;
    case ePlaceCity: m_site->setCity(value); break;
    case ePlaceMiscText: m_site->setMiscText(value); break;
    default: qCritical() << "Dont know how to handle text event" << source; break;
    }

}

void PlaceController::intEvent(EUISource source, int value)
{
    switch(source)
    {
    case ePlaceList: m_site = Singletons::model()->getSite(value); break;
    default: qCritical() << "Dont know how to handle int event" << source; break;
    }
    sendNotificationToObservers(Controller::ePlaceUpdated);
}

QDate PlaceController::getDateValue(EUISource source)
{
    return QDate();
}

QTime PlaceController::getTimeValue(EUISource source)
{
    return QTime();
}

bool PlaceController::getBooleanValue(EUISource source)
{
    switch(source)
    {
    case ePlaceInvisible: return m_site->getInvisible(); break;
    default: break;
    }
    return false;
}

int PlaceController::getIntValue(EUISource source)
{
    return 0;
}

QString PlaceController::getTextValue(EUISource source)
{
    switch(source)
    {
    case ePlaceName: return m_site->getName(); break;
    case ePlaceCity: return m_site->getCity(); break;
    case ePlaceMiscText: return m_site->getMiscText(); break;
    default: break;
    }
    return QString();
}

QMap<QString, int> PlaceController::getPlaceList()
{
    QMap<QString, int> retval;
    QMap<int, Site*> sitelist = Singletons::model()->getSites();

    for(QMap<int, Site*>::iterator iter = sitelist.begin(); iter != sitelist.end(); iter++)
    {
        Site* site = iter.value();
        QString sitename = site->getName()+" "+site->getCity();
        retval[sitename] = site->getId();
    }
    return retval;
}
