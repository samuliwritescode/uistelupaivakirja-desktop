#include "placecontroller.h"
#include "singletons.h"

PlaceController::PlaceController():
        m_place(NULL)
{    
}

void PlaceController::buttonEvent(EUISource source)
{
    switch(source)
    {
    case ePlaceNew:
        //Singletons::model()->commit(m_place);
        m_place = Singletons::model()->getPlace();
        break;
    case ePlaceDelete:
        Singletons::model()->remove(m_place);
        break;
    default: qCritical() << "Dont know how to handle button event" << source; break;
    }

    sendNotificationToObservers(Controller::ePlaceListUpdated);
}

void PlaceController::booleanEvent(EUISource source, bool value)
{
    switch(source)
    {
    case ePlaceInvisible: m_place->setInvisible(value); break;
    default: qCritical() << "Dont know how to handle boolean event" << source; break;
    }
    sendNotificationToObservers(Controller::ePlaceUpdated);
}

void PlaceController::textEvent(EUISource source, const QString& value)
{

    switch(source)
    {
    case ePlaceName: m_place->setName(value); break;
    case ePlaceCity: m_place->setCity(value); break;
    case ePlaceMiscText: m_place->setMiscText(value); break;
    default: qCritical() << "Dont know how to handle text event" << source; break;
    }
    sendNotificationToObservers(Controller::ePlaceListUpdated);
}

void PlaceController::intEvent(EUISource source, int value)
{
    switch(source)
    {
    case ePlaceList: m_place = Singletons::model()->getPlace(value); break;
    default: qCritical() << "Dont know how to handle int event" << source; break;
    }
    sendNotificationToObservers(Controller::ePlaceUpdated);
}

bool PlaceController::getBooleanValue(EUISource source)
{
    switch(source)
    {
    case ePlaceInvisible: return m_place->getInvisible(); break;
    default: break;
    }
    return false;
}

QString PlaceController::getTextValue(EUISource source)
{
    switch(source)
    {
    case ePlaceName: return m_place->getName(); break;
    case ePlaceCity: return m_place->getCity(); break;
    case ePlaceMiscText: return m_place->getMiscText(); break;
    default: break;
    }
    return QString();
}

QList<QPair<QString, int> > PlaceController::getPlaceList()
{
    QList<QPair<QString, int> > retval;
    QMap<int, Place*> placelist = Singletons::model()->getPlaces();

    for(QMap<int, Place*>::iterator iter = placelist.begin(); iter != placelist.end(); iter++)
    {
        Place* place = iter.value();
        QPair<QString, int> pair;
        pair.first = place->getName()+" "+place->getCity();
        pair.second = place->getId();
        retval.push_back(pair);
    }
    return retval;
}
