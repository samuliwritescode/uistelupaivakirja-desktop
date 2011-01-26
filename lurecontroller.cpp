#include "lurecontroller.h"

LureController::LureController()
{

}

void LureController::buttonEvent(EUISource source)
{
    switch(source)
    {
    case eLureNew: break;
    case eLureDelete: break;
    default: break;
    }
    sendNotificationToObservers(Controller::eLureListUpdated);
}

void LureController::booleanEvent(EUISource source, bool value)
{
    switch(source)
    {
    case eLureFavorite: break;
    default: break;
    }
    sendNotificationToObservers(Controller::eLureUpdated);
}

void LureController::dateEvent(EUISource source, const QDate& value)
{

}

void LureController::textEvent(EUISource source, const QString& value)
{
    switch(source)
    {
    case eLureMaker: break;
    case eLureModel: break;
    case eLureSize: break;
    case eLureColor: break;
    default: break;
    }
    sendNotificationToObservers(Controller::eLureUpdated);
}

void LureController::intEvent(EUISource source, int value)
{
    switch(source)
    {
    case eLureList: break;
    default: break;
    }
    sendNotificationToObservers(Controller::eLureUpdated);
}

QDate LureController::getDateValue(EUISource source)
{
    return QDate();
}

bool LureController::getBooleanValue(EUISource source)
{
    return false;
}

int LureController::getIntValue(EUISource source)
{
    return 0;
}

QString LureController::getTextValue(EUISource source)
{
    return QString();
}

double LureController::getDoubleValue(EUISource source)
{
    return 0;
}
