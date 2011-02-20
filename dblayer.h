#ifndef DBLAYER_H
#define DBLAYER_H
#include <QString>
#include <QDir>
#include "trollingobject.h"
#include "trollingobjectfactory.h"

class DBLayer
{
public:
    DBLayer(const QString& p_storePath);

    bool storeObject(TrollingObject* p_object);
    bool loadObjects(const QString& p_type, TrollingObjectFactory* p_factory, int id = -1);
    bool removeObject(TrollingObject* p_object);
    QList<int> getIds(const QString& p_type);

private:
    QDir m_storeDir;
};

#endif // DBLAYER_H
