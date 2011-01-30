#ifndef PLACECONTROLLER_H
#define PLACECONTROLLER_H

#include "controller.h"
#include "place.h"

class PlaceController : public Controller
{
public:
    PlaceController();
    virtual void buttonEvent(EUISource source);
    virtual void booleanEvent(EUISource source, bool value);
    virtual void textEvent(EUISource source, const QString& value);
    virtual void intEvent(EUISource source, int value);

    virtual bool getBooleanValue(EUISource source);
    virtual QString getTextValue(EUISource source);

    QList<QPair<QString, int> > getPlaceList();

private:
    Place* m_place;
};

#endif // PLACECONTROLLER_H
