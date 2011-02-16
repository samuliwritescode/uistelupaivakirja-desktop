#include <QDir>
#include <QDebug>
#include "dblayer.h"
#include "xmlwriter.h"

DBLayer::DBLayer(const QString& p_storePath):
m_storeDir(p_storePath)
{

    if(!m_storeDir.exists("."))
    {
        m_storeDir.mkpath(".");
    }
}

bool DBLayer::storeObject(TrollingObject* p_object)
{
    XMLWriter writer(m_storeDir.absolutePath()+"/"+p_object->getType()+".xml");
    return writer.write(p_object);
}

void DBLayer::removeObject(TrollingObject* p_object)
{
    XMLWriter writer(m_storeDir.absolutePath()+"/"+p_object->getType()+".xml");
    writer.remove(p_object);
}

QList<int> DBLayer::getIds(const QString& p_type)
{
    XMLWriter writer(m_storeDir.absolutePath()+"/"+p_type+".xml");
    return writer.getIds(p_type);
}

bool DBLayer::loadObjects(const QString& p_type, TrollingObjectFactory* p_factory, int id)
{
    XMLWriter writer(m_storeDir.absolutePath()+"/"+p_type+".xml");
    return writer.loadAll(p_type, p_factory, id);
}
