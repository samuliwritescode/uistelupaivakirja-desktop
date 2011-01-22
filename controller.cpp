#include <QDebug>
#include "controller.h"

Controller::Controller(QObject *parent) :
    QObject(parent)
{
}


void Controller::sendNotificationToObservers(ENotificationType type)
{
    emit observerNotification(static_cast<int>(type));
}
