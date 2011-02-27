#include "placecontroller.h"
#include "singletons.h"
#include "trollingexception.h"

PlaceController::PlaceController():
        m_place(NULL)
{    
}

void PlaceController::buttonEvent(EUISource source)
{
    try
    {
        switch(source)
        {
        case ePlaceUndo:
            {
                int id = m_place->getId();
                Singletons::model()->reset(m_place);
                m_place = Singletons::model()->getPlace(id);
            }
            break;
        case ePlaceSave: Singletons::model()->commit(m_place); break;
        case ePlaceNew:
            m_place = Singletons::model()->getPlace();
            break;
        case ePlaceDelete:
            Singletons::model()->remove(m_place);
            m_place = NULL;
            break;
        default: qCritical() << "Dont know how to handle button event" << source; break;
        }
    }
    catch(TrollingException e)
    {
        showErrorMessage(e.toString());
    }

    sendNotificationToObservers(Controller::ePlaceListUpdated);
    sendNotificationToObservers(Controller::ePlaceUpdated);
}

void PlaceController::booleanEvent(EUISource source, bool value)
{
    if(!m_place) return;

    switch(source)
    {
    case ePlaceInvisible: m_place->setInvisible(value); break;
    default: qCritical() << "Dont know how to handle boolean event" << source; break;
    }
    sendNotificationToObservers(ePlaceInternalUpdate);
}

void PlaceController::textEvent(EUISource source, const QString& value)
{
    if(!m_place) return;

    switch(source)
    {
    case ePlaceName: m_place->setName(value); break;
    case ePlaceCity: m_place->setCity(value); break;
    case ePlaceMiscText: m_place->setMiscText(value); break;
    default: qCritical() << "Dont know how to handle text event" << source; break;
    }
    sendNotificationToObservers(ePlaceInternalUpdate);
}

void PlaceController::selectPlace(int value)
{
    if(m_place && m_place->isUnsaved())
    {
        int choice = showChoiseMessage(tr("Nykyinen kalapaikka on tallentamatta. Haluatko tallentaa muutokset?"));
        if(choice == MessageDisplay::eCancel)
        {
            sendNotificationToObservers(Controller::ePlaceListUpdated);
            return;
        }
        else if(choice == MessageDisplay::eYes)
        {
            Singletons::model()->commit(m_place);
        }
        else
        {
            Singletons::model()->reset(m_place);
        }
        sendNotificationToObservers(Controller::ePlaceListUpdated);
    }
    m_place = Singletons::model()->getPlace(value);
}

void PlaceController::intEvent(EUISource source, int value)
{
    switch(source)
    {
    case ePlaceList: selectPlace(value); break;
    default: qCritical() << "Dont know how to handle int event" << source; break;
    }
    sendNotificationToObservers(Controller::ePlaceUpdated);
}

bool PlaceController::getBooleanValue(EUISource source)
{
    if(!m_place) return false;

    switch(source)
    {
    case ePlaceList: return true; break;
    case ePlaceInvisible: return m_place->getInvisible(); break;
    case eUnsavedChanges: return m_place->isUnsaved(); break;
    default: break;
    }
    return false;
}

QString PlaceController::getTextValue(EUISource source)
{
    if(!m_place) return QString();
    switch(source)
    {
    case ePlaceName: return m_place->getName(); break;
    case ePlaceCity: return m_place->getCity(); break;
    case ePlaceMiscText: return m_place->getMiscText(); break;
    default: break;
    }
    return QString();
}

int PlaceController::getIntValue(EUISource source)
{
    if(!m_place) return 0;

    switch(source)
    {
    case ePlaceList: return m_place->getId();
    default: break;
    }
    return 0;
}

QList<QPair<QString, int> > PlaceController::getPlaceListShort()
{
    QList<QPair<QString, int> > retval;
    QMap<int, Place*> placelist = Singletons::model()->getPlaces();

    for(QMap<int, Place*>::iterator iter = placelist.begin(); iter != placelist.end(); iter++)
    {
        Place* place = iter.value();
        if(place->getInvisible())
            continue;

        QPair<QString, int> pair;
        pair.first = place->getName()+" "+place->getCity();
        pair.second = place->getId();
        retval.push_back(pair);
    }
    return retval;
}

QList<QList<QString> > PlaceController::getPlaceListFull()
{
    QList<QList<QString> > retval;
    QMap<int, Place*> placelist = Singletons::model()->getPlaces();

    for(QMap<int, Place*>::iterator iter = placelist.begin(); iter != placelist.end(); iter++)
    {
        Place* place = iter.value();
        QList<QString> placehash;
        placehash.push_back(place->getName());
        placehash.push_back(place->getCity());
        if(place->getInvisible())
            placehash.push_back(tr("ei näy"));
        else
            placehash.push_back(tr(""));

        placehash.push_back(place->getMiscText());
        placehash.push_back(QString::number(place->getId()));
        retval.push_back(placehash);
    }
    return retval;
}

QStringList PlaceController::getPlaceColumns()
{
    QStringList retval;
    retval << tr("Nimi") <<
           tr("Paikkakunta") <<
           tr("Näkyy") <<
           tr("Muuta");
    return retval;
}

