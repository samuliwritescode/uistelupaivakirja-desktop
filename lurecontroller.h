#ifndef LURECONTROLLER_H
#define LURECONTROLLER_H
#include "controller.h"
#include "lure.h"

class LureController : public Controller
{
public:
    LureController();

    virtual void buttonEvent(EUISource source);
    virtual void booleanEvent(EUISource source, bool value);
    virtual void dateEvent(EUISource source, const QDate& value);
    virtual void textEvent(EUISource source, const QString& value);
    virtual void intEvent(EUISource source, int value);

    virtual QDate getDateValue(EUISource source);
    virtual bool getBooleanValue(EUISource source);
    virtual int getIntValue(EUISource source);
    virtual QString getTextValue(EUISource source);

    QMap<QString, int> getLureList();

private:
    Lure* m_lure;
};

#endif // LURECONTROLLER_H
