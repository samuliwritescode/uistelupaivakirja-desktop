#ifndef MESSAGEDISPLAY_H
#define MESSAGEDISPLAY_H
#include <QString>

class MessageDisplay
{
public:
    enum EChoices {eCancel, eNo, eYes};
    virtual void showErrorMessage(const QString&) = 0;
    virtual bool showConfirmationMessage(const QString&) = 0;
    virtual EChoices showChoiceMessage(const QString&) = 0;
    virtual void showNotificationMessage(const QString&) = 0;
    virtual void showStatusMessage(const QString&, bool) = 0;
protected:
    MessageDisplay();
};

#endif // MESSAGEDISPLAY_H
