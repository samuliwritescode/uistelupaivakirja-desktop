#include <QDebug>
#include "controller.h"

Controller::Controller(QObject *parent) :
    QObject(parent),
    m_display(NULL)
{
}

void Controller::setMessageDisplay(MessageDisplay* p_display)
{
    m_display = p_display;
}

MessageDisplay* Controller::display()
{
    return m_display;
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

void Controller::showNotificationMessage(const QString& notify)
{
    if(display())
    {
        display()->showNotificationMessage(notify);
    }
}

void Controller::showErrorMessage(const QString& error)
{
    if(display())
    {
        display()->showErrorMessage(error);
    }
}

bool Controller::showConfirmationMessage(const QString& message)
{
    if(display())
    {
        return display()->showConfirmationMessage(message);
    }
    return false;
}

MessageDisplay::EChoices Controller::showChoiseMessage(const QString& message)
{
    if(display())
    {
        return display()->showChoiceMessage(message);
    }
    return MessageDisplay::eCancel;
}

void Controller::showStatusMessage(const QString& message, bool stay)
{
    if(display())
    {
        display()->showStatusMessage(message, stay);
    }
}
