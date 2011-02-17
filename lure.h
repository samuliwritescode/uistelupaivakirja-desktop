#ifndef LURE_H
#define LURE_H
#include "trollingobject.h"

class Lure : public TrollingObject
{
public:
    Lure();

    void setMaker(const QString&);
    void setModel(const QString&);
    void setSize(const QString&);
    void setColor(const QString&);
    void setFavorite(bool);
    void setLureType(const QString&);

    QString getMaker();
    QString getModel();
    QString getSize();
    QString getColor();
    QString getLureType();
    bool getFavorite();
};

#endif // LURE_H
