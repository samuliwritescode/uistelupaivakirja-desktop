#include <QDebug>
#include "controller.h"

Controller::Controller(QObject *parent) :
    QObject(parent)
{
}

void Controller::buttonEvent(EUISource)
{
    qCritical() << "Controller::buttonEvent";
}

void Controller::booleanEvent(EUISource, bool)
{
    qCritical() << "Controller::booleanEvent";
}

void Controller::dateEvent(EUISource, const QDate&)
{
    qCritical() << "Controller::dateEvent";
}

void Controller::timeEvent(EUISource, const QTime&)
{
    qCritical() << "Controller::timeEvent";
}

void Controller::textEvent(EUISource, const QString&)
{
    qCritical() << "Controller::textEvent";
}

void Controller::intEvent(EUISource, int)
{
    qCritical() << "Controller::intEvent";
}

QDate Controller::getDateValue(EUISource)
{
    qCritical() << "Controller::getDateValue";
    return QDate();
}

QTime Controller::getTimeValue(EUISource)
{
    qCritical() << "Controller::getTimeValue";
    return QTime();
}

bool Controller::getBooleanValue(EUISource)
{
    qCritical() << "Controller::getBooleanValue";
    return false;
}

int Controller::getIntValue(EUISource)
{
    qCritical() << "Controller::getIntValue";
    return -1;
}

QString Controller::getTextValue(EUISource)
{
    qCritical() << "Controller::getTextValue";
    return QString();
}

void Controller::sendNotificationToObservers(ENotificationType type)
{
    emit observerNotification(static_cast<int>(type));
}
