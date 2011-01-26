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

    QString getMaker();
    QString getModel();
    QString getSize();
    QString getColor();
    bool getFavorite();

};

#endif // LURE_H
