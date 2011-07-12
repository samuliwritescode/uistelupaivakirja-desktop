#include "simpletrollingobjectfactory.h"
#include "lure.h"
#include "trip.h"
#include "place.h"

SimpleTrollingObjectFactory::SimpleTrollingObjectFactory():
        m_collection(NULL)
{
}


TrollingObject* SimpleTrollingObjectFactory::createTrollingObject(const QString& p_type)
{
    TrollingObject* object = NULL;
    if(p_type == Lure().getType())
    {
        object = new Lure();
    } else if(p_type == Trip().getType())
    {
        object = new Trip();
    } else if(p_type == Place().getType())
    {
        object = new Place();
    }

    if(object && m_collection)
    {
        m_collection->push_back(object);
    }

    return object;
}

void SimpleTrollingObjectFactory::setPushToCollection(QList<TrollingObject*>* collection)
{
    m_collection = collection;
}
