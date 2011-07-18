#ifndef PLACE_H
#define PLACE_H
#include "trollingobject.h"

class Place : public TrollingObject
{
public:
    Place();
    void setName(const QString&);
    void setCity(const QString&);
    void setMiscText(const QString&);
    void setInvisible(bool);
    virtual QString toString();

    QString getName();
    QString getCity();
    QString getMiscText();
    bool getInvisible();
};

#endif // PLACE_H
