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
        m_trips[id] = trip;
    }
}

TrollingModel::~TrollingModel()
{
    delete m_DBLayer;
    foreach(Trip* trip, m_trips)
    {
        delete trip;
    }
    m_trips.clear();
}

Trip* TrollingModel::getTrip(int id)
{
    if(id < 0 )
        return new Trip();

    if(m_trips.contains(id))
        return m_trips[id];
    else
        return NULL;
}
QMap<int, Trip*> TrollingModel::getTrips()
{
    return m_trips;
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
