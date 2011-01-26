#include <QDir>
#include <QDebug>
#include "trollingmodel.h"

TrollingModel::TrollingModel(QObject *parent) :
    QObject(parent)
{

}

void TrollingModel::initialize()
{
    m_DBLayer = new DBLayer(QDir::homePath()+"/uistelu/database");

    QList<int> lureIds = m_DBLayer->getIds(Lure().getType());
    foreach(int id, lureIds)
    {
        Lure* lure = new Lure();
        m_DBLayer->loadObject(id, lure);
        m_trollingobjects.push_back( lure );
    }

    QList<int> tripIds = m_DBLayer->getIds(Trip().getType());
    foreach(int id, tripIds)
    {
        Trip* trip = new Trip();
        m_DBLayer->loadObject(id, trip);
        m_trollingobjects.push_back( trip );
    }
}

TrollingModel::~TrollingModel()
{
    delete m_DBLayer;
    foreach(TrollingObject* object, m_trollingobjects)
    {
        delete object;
    }
    m_trollingobjects.clear();
}

Trip* TrollingModel::getTrip(int id)
{
    if(id < 0 )
    {
        Trip* trip = new Trip();
        m_trollingobjects.push_back(trip);
        return trip;
    }

    return reinterpret_cast<Trip*>(getTrollingObject("trip", id));
}

QMap<int, Trip*> TrollingModel::getTrips()
{
    QMap<int, Trip*> retval;
    foreach(TrollingObject* object, m_trollingobjects)
    {
        if(object->getType() == "trip")
        {
            retval[object->getId()] = reinterpret_cast<Trip*>(object);
        }
    }

    return retval;
}

TrollingObject* TrollingModel::getTrollingObject(const QString& type, int id)
{
    foreach(TrollingObject* object, m_trollingobjects)
    {
        if(object->getType() == type && object->getId() == id)
        {
            return object;
        }
    }
    return NULL;
}

Lure* TrollingModel::getLure(int id)
{
    if(id < 0 )
    {
        Lure* lure = new Lure();
        m_trollingobjects.push_back(lure);
        return lure;
    }

    return reinterpret_cast<Lure*>(getTrollingObject("lure", id));
}

QMap<int, Lure*> TrollingModel::getLures()
{
    QMap<int, Lure*> retval;
    foreach(TrollingObject* object, m_trollingobjects)
    {
        if(object->getType() == "lure")
        {
            retval[object->getId()] = reinterpret_cast<Lure*>(object);
        }
    }

    return retval;
}

Site* TrollingModel::getSite(int id)
{
    if(id < 0)
        return new Site();

    return NULL;
}

Species* TrollingModel::getSpecies(int id)
{
    if(id < 0)
        return new Species();

    return NULL;
}

Method* TrollingModel::getMethod(int id)
{
    if(id < 0)
        return new Method();

    return NULL;
}

int TrollingModel::commit(TrollingObject* object)
{   
    if(object == NULL)
       return -2;

    m_DBLayer->storeObject(object);
    return object->getId();
}

void TrollingModel::remove(TrollingObject* p_object)
{
    if(p_object == NULL)
       return;

    m_DBLayer->removeObject(p_object);
    for(int loop=0; loop < m_trollingobjects.size(); loop++)
    {
        if(m_trollingobjects[loop] == p_object)
        {
            m_trollingobjects.removeAt(loop);
        }
    }

    delete p_object;
}
