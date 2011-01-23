#include <QDir>
#include "trollingmodel.h"

TrollingModel::TrollingModel(QObject *parent) :
    QObject(parent)
{
    m_DBLayer = new DBLayer(QDir::homePath()+"/uistelu/database");

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

    QMap<int, Trip*> trips = getTrips();
    if(trips.contains(id))
        return trips[id];
    else
        return NULL;
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

Lure* TrollingModel::getLure(int id)
{
    if(id < 0)
        return new Lure();

    return NULL;
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
