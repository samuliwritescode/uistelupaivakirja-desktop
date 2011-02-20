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
    void setNotVisible(bool);
    void setLureType(const QString&);
    void setNickName(const QString&);

    QString getMaker();
    QString getModel();
    QString getSize();
    QString getColor();
    QString getLureType();
    QString getNickName();
    bool getFavorite();
    bool getNotVisible();
};

#endif // LURE_H
