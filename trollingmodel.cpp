#include <QDir>
#include <QDebug>
#include "trollingexception.h"
#include "trollingmodel.h"

TrollingModel::TrollingModel(QObject *parent) :
    QObject(parent)
{
    m_filePath = QDir::homePath()+"/uistelu/";
}

void TrollingModel::initialize()
{
    m_DBLayer = new DBLayer(m_filePath+"/database");
    qDebug() << "start loading";
    m_DBLayer->loadObjects(Lure().getType(), this);
    m_DBLayer->loadObjects(Trip().getType(), this);
    m_DBLayer->loadObjects(Place().getType(), this);
    qDebug() << "finished loading";
}

TrollingObject* TrollingModel::createTrollingObject(const QString& p_type)
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

    if(object)
    {
        m_trollingobjects.push_back(object);
    }

    return object;
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
    if(id < 0 && getTrollingObject("trip", id) == NULL)
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
    if(id < 0 && getTrollingObject("lure", id) == NULL )
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

QMap<int, Place*> TrollingModel::getPlaces()
{
    QMap<int, Place*> retval;
    foreach(TrollingObject* object, m_trollingobjects)
    {
        if(object->getType() == "place")
        {
            retval[object->getId()] = reinterpret_cast<Place*>(object);
        }
    }

    return retval;
}


Place* TrollingModel::getPlace(int id)
{
    if(id < 0 && getTrollingObject("place", id) == NULL)
    {
        Place* place = new Place();
        m_trollingobjects.push_back(place);
        return place;
    }

    return reinterpret_cast<Place*>(getTrollingObject("place", id));
}

int TrollingModel::commit(TrollingObject* object)
{   
    if(object == NULL)
       return -2;

    if(object->valid() != QString())
        throw TrollingException(object->valid());

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
            break;
        }
    }

    delete p_object;
}

void TrollingModel::reset(TrollingObject* p_object)
{
    if(p_object == NULL)
        return;

    m_DBLayer->loadObjects(p_object->getType(), this, p_object->getId());
    for(int loop=0; loop < m_trollingobjects.size(); loop++)
    {
        if(m_trollingobjects[loop] == p_object)
        {
            m_trollingobjects.removeAt(loop);
            break;
        }
    }

    delete p_object;
}

QString TrollingModel::importFile(TrollingObject* p_object, const QString& p_file)
{
    QFileInfo fileinfo(p_file);

    QString newPath = m_filePath+p_object->getType()+"/"+QString::number(p_object->getId())+"/";
    QString newFile = newPath+fileinfo.fileName();
    QDir dir(newPath);
    if(!dir.mkpath("."))
    {
        QString error = QObject::tr("En pystynyt luomaan kansiota, jonne tallentaa antamasi tiedosto. Katso, että hakemisto ");
        error += newPath;
        error +=  QObject::tr(" on olemassa ja sinne on kirjoitusoikeudet myös ohjelman käyttäjäoikeuksilla. Windows 7:ssä tämä voi vaatia antamaan ylläpitäjäoikeudet Uistelupäiväkirjalle.");
        throw TrollingException(error);
    }

    if(QFile::exists(newFile))
    {
        if(!QFile::remove(newFile))
        {
            QString error = QObject::tr("Antamasi tiedosto on jo olemassa ja sen poistaminen ei onnistu. Tiedosto: ");
            error += newFile;
            throw TrollingException(error);
        }
    }

    if(!QFile::copy(p_file, newFile))
    {
        QString error = QObject::tr("Tiedoston ");
        error += p_file;
        error += QObject::tr(" liittäminen ei onnistunut");
        throw TrollingException(error);
    }

    return newFile;
}
