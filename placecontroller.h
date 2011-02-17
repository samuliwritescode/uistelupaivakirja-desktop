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
    virtual int getIntValue(EUISource);

    QList<QPair<QString, int> > getPlaceListShort();
    QList<QList<QString> > getPlaceListFull();
    QStringList getPlaceColumns();

private:
    void selectPlace(int value);
    Place* m_place;
};

#endif // PLACECONTROLLER_H
