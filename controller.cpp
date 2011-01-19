#include "controller.h"

Controller::Controller(QObject *parent) :
    QObject(parent)
{
}


void Controller::sendNotificationToObservers()
{
    emit observerNotification();
}
