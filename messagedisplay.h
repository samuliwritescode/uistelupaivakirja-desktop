#ifndef MESSAGEDISPLAY_H
#define MESSAGEDISPLAY_H
#include <QString>

class MessageDisplay
{
public:
    virtual void showErrorMessage(const QString&) = 0;
    virtual bool showConfirmationMessage(const QString&) = 0;
    virtual int showChoiceMessage(const QString&) = 0;
protected:
    MessageDisplay();
};

#endif // MESSAGEDISPLAY_H
