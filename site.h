#ifndef SITE_H
#define SITE_H
#include "trollingobject.h"

class Site : public TrollingObject
{
public:
    Site();
    void setName(const QString&);
    void setCity(const QString&);
    void setMiscText(const QString&);
    void setInvisible(bool);

    QString getName();
    QString getCity();
    QString getMiscText();
    bool getInvisible();
};

#endif // SITE_H
