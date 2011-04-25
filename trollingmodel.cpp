#include <QDir>
#include <QSettings>
#include <QDebug>
#include <QStringList>
#include "trollingexception.h"
#include "trollingmodel.h"
#include "singletons.h"
#include "alternative.h"

TrollingModel::TrollingModel(QObject *parent) :
    QObject(parent)
{
    m_readFromMobile = false;
    QSettings settings;
    m_filePath = settings.value("ProgramFolder").toString();
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
        if(m_readFromMobile)
        {
            m_trollingobjectsmobile.push_back(object);
            return object;
        }
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

void TrollingModel::syncMobile()
{
    QSettings settings;
    QString memCard = settings.value("MobileFolder").toString();
    if(!memCard.isEmpty())
    {
        if(!QDir().exists(memCard))
        {
            throw TrollingException(tr("Muistikorttia ei löydy. Tarkista puhelimen kytkentä."));
        }

        QFile::remove(memCard+"/uistelu/lure.xml");
        QFile::remove(memCard+"/uistelu/place.xml");
        QFile::remove(memCard+"/uistelu/spinneritems.xml");
        if(!QFile::copy(m_filePath+"/database/lure.xml", memCard+"/uistelu/lure.xml"))
        {
            throw TrollingException(tr("En kykene kopioimaan viehekantaa muistikortille. Tarkista ettei kortti ole kirjoitussuojattu"));
        }

        if(!QFile::copy(m_filePath+"/database/place.xml", memCard+"/uistelu/place.xml"))
        {
             throw TrollingException(tr("En kykene kopioimaan paikkakantaa muistikortille. Tarkista ettei kortti ole kirjoitussuojattu"));
        }

        DBLayer dblayerMobile(memCard+"/uistelu/");

        foreach(TrollingObject* object, m_trollingobjectsmobile)
        {
            delete object;
        }

        m_trollingobjectsmobile.clear();

        m_readFromMobile = true;
        dblayerMobile.loadObjects(Trip().getType(), this);
        m_readFromMobile = false;
        int maxId = 1;
        foreach(TrollingObject* object, m_trollingobjects)
        {
            if(object->getType() == Trip().getType() &&
               object->getId() > maxId)
            {
                maxId = object->getId();
            }
        }

        foreach(TrollingObject* object, m_trollingobjectsmobile)
        {
            maxId++;
            qDebug() << "setting id to mobile trip" << maxId;
            object->setId(maxId);
            m_trollingobjects.push_back(object);
            m_DBLayer->storeObject(object);
        }
        m_trollingobjectsmobile.clear();

        QFile::remove(memCard+"/uistelu/trip.xml");

        QStringList specielist = Singletons::tripController()->getAlternatives(eSpecies);
        QStringList getterlist = Singletons::tripController()->getAlternatives(eGetter);
        QStringList methodlist = Singletons::tripController()->getAlternatives(eMethod);
        foreach(QString string, specielist)
        {
            dblayerMobile.storeObject(&Alternative("species", string));
        }

        foreach(QString string, getterlist)
        {
            dblayerMobile.storeObject(&Alternative("getter", string));
        }

        foreach(QString string, methodlist)
        {
            dblayerMobile.storeObject(&Alternative("method", string));
        }
    }
    else
    {
        throw TrollingException(tr("Muistikortin sijaintia ei ole asetettu. Aseta se ensin asetusvälilehdellä."));
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
    if(id == 0)
        return NULL;

    if(m_fasterHash.contains(id) && m_fasterHash[id]->getType() == type)
    {
        return m_fasterHash[id];
    }

    foreach(TrollingObject* object, m_trollingobjects)
    {
        if(object->getType() == type && object->getId() == id)
        {
            m_fasterHash[id] = object;
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

    if(!m_DBLayer->storeObject(object))
    {
        throw TrollingException(tr("En pysty tallentamaan. Katsopas, ettei levy ole täynnä tai ohjelmalla on tallennusoikeudet."));
    }
    return object->getId();
}

void TrollingModel::remove(TrollingObject* p_object)
{
    if(p_object == NULL)
       return;

    if(p_object->getId() >= 0 && !m_DBLayer->removeObject(p_object))
    {
        throw TrollingException(tr("En pysty poistamaan. Katsopas, ettei levy ole täynnä tai ohjelmalla on tallennusoikeudet."));
    }

    for(int loop=0; loop < m_trollingobjects.size(); loop++)
    {
        if(m_trollingobjects[loop] == p_object)
        {
            m_trollingobjects.removeAt(loop);
            break;
        }
    }

    if(m_fasterHash.contains(p_object->getId()) &&
       m_fasterHash[p_object->getId()] == p_object)
    {
        m_fasterHash.remove(p_object->getId());
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

    if(m_fasterHash.contains(p_object->getId()) &&
       m_fasterHash[p_object->getId()] == p_object)
    {
        m_fasterHash.remove(p_object->getId());
    }

    delete p_object;
}

QString TrollingModel::importFile(TrollingObject* p_object, const QString& p_file)
{
    QFileInfo fileinfo(p_file);

    QString newPath = m_filePath+p_object->getType()+"/"+QString::number(p_object->getId())+"/";
    QString newFile = newPath+fileinfo.fileName();
    QDir dir(newPath);

    if(!fileinfo.exists())
    {
        QString error = QObject::tr("Antamaasi tiedostoa ei löydy: ");
        error += p_file;
        throw TrollingException(error);
    }

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
