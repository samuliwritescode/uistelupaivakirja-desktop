#include "trollingmodel.h"

TrollingModel::TrollingModel(QObject *parent) :
    QObject(parent)
{
}

Trip* TrollingModel::getTrip(int id)
{
    if(id < 0)
        return new Trip();

    return NULL;
}

Fish* TrollingModel::getFish(int id)
{
    if(id < 0)
        return new Fish();

    return NULL;
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

   // m_xmlWriter.write("object.xml", object);
}
