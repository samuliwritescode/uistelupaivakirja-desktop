#include "lurecontroller.h"
#include "singletons.h"

LureController::LureController():
        m_lure(NULL)
{
    m_lure = Singletons::model()->getLure();
}

void LureController::buttonEvent(EUISource source)
{
    switch(source)
    {
    case eLureNew:
        Singletons::model()->commit(m_lure);
        m_lure = Singletons::model()->getLure(); break;
    case eLureDelete:
        Singletons::model()->remove(m_lure);
        m_lure = Singletons::model()->getLure(); break;
    default: break;
    }
    sendNotificationToObservers(Controller::eLureUpdated);
    sendNotificationToObservers(Controller::eLureListUpdated);
}

void LureController::booleanEvent(EUISource source, bool value)
{
    if(!m_lure) return;
    switch(source)
    {
    case eLureFavorite: m_lure->setFavorite(value); break;
    default: break;
    }
    sendNotificationToObservers(Controller::eLureUpdated);
}

void LureController::dateEvent(EUISource source, const QDate& value)
{

}

void LureController::timeEvent(EUISource source, const QTime& value)
{

}

void LureController::textEvent(EUISource source, const QString& value)
{
    if(!m_lure) return;

    switch(source)
    {
    case eLureMaker: m_lure->setMaker(value);break;
    case eLureModel: m_lure->setModel(value); break;
    case eLureSize: m_lure->setSize(value); break;
    case eLureColor: m_lure->setColor(value); break;
    default: break;
    }
    sendNotificationToObservers(Controller::eLureUpdated);
}

void LureController::intEvent(EUISource source, int value)
{
    if(!m_lure) return;

    switch(source)
    {
    case eLureList: m_lure = Singletons::model()->getLure(value); break;
    default: break;
    }
    sendNotificationToObservers(Controller::eLureUpdated);
}

QDate LureController::getDateValue(EUISource source)
{
    return QDate();
}

QTime LureController::getTimeValue(EUISource source)
{
    return QTime();
}

bool LureController::getBooleanValue(EUISource source)
{
    if(!m_lure) return false;
    switch(source)
    {
    case eLureFavorite: return m_lure->getFavorite(); break;
    default: break;
    }
    return false;
}

int LureController::getIntValue(EUISource source)
{
    return 0;
}

QString LureController::getTextValue(EUISource source)
{
    if(!m_lure) return QString();

    switch(source)
    {
    case eLureMaker: return m_lure->getMaker();break;
    case eLureModel: return m_lure->getModel(); break;
    case eLureSize: return m_lure->getSize(); break;
    case eLureColor: return m_lure->getColor(); break;
    default: break;
    }
    return QString();
}

QList<QPair<QString, int> > LureController::getLureList()
{
    QList<QPair<QString, int> > retval;
    QMap<int, Lure*> lurelist = Singletons::model()->getLures();

    for(QMap<int, Lure*>::iterator iter = lurelist.begin(); iter != lurelist.end(); iter++)
    {
        Lure* lure = iter.value();
        QPair<QString, int> pair;
        pair.first = lure->getMaker()+" "+lure->getModel()+" "+lure->getSize()+" "+lure->getColor();
        pair.second = lure->getId();

        retval.push_back(pair);
    }
    return retval;
}
