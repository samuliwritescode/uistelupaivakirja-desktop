#include <QDir>
#include <QDebug>
#include "dblayer.h"
#include "xmlwriter.h"

DBLayer::DBLayer(const QString& p_storePath, QObject *parent):
QObject(parent),
m_storeDir(p_storePath)
{
    if(!m_storeDir.exists("."))
    {
        m_storeDir.mkpath(".");
    }
}

DBLayer::~DBLayer()
{

}

bool DBLayer::storeObject(TrollingObject* p_object)
{
    XMLWriter writer(m_storeDir.absolutePath()+"/"+p_object->getType()+".xml");
    bool bOk = writer.write(p_object);
    if(bOk)
    {
        emit storeSignal(p_object->getId(), p_object->getType());
    }
    return bOk;
}

bool DBLayer::removeObject(TrollingObject* p_object)
{
    XMLWriter writer(m_storeDir.absolutePath()+"/"+p_object->getType()+".xml");
    return writer.remove(p_object);
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

void DBLayer::setRevision(int revision, const QString& p_type)
{
   XMLWriter writer(m_storeDir.absolutePath()+"/"+p_type+".xml");
   writer.setRevision(revision);
}

int DBLayer::getRevision(const QString& p_type)
{
    XMLWriter writer(m_storeDir.absolutePath()+"/"+p_type+".xml");
    return writer.getRevision();
}
