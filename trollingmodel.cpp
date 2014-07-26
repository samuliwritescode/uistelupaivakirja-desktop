#include <QDir>
#include <QSettings>
#include <QDebug>
#include <QStringList>
#include "trollingexception.h"
#include "trollingmodel.h"
#include "singletons.h"
#include "alternative.h"
#include "simpletrollingobjectfactory.h"
#include "reloadingtrollingobjectfactory.h"

TrollingModel::TrollingModel(QObject *parent) :
    QObject(parent)
{
    m_factory.setPushToCollection(&m_trollingobjects);
    QSettings settings;
    m_filePath = settings.value("ProgramFolder").toString();       
}

void TrollingModel::initialize()
{
    m_DBLayer = new DBLayer(m_filePath+"/database");
    connect(m_DBLayer, SIGNAL(storeSignal(int, QString)), &m_journal, SLOT(addJournal(int, QString)));
    connect(&m_synchronizer, SIGNAL(uploadDone()), this, SLOT(resetJournal()));

    qDebug() << "start loading";
    m_DBLayer->loadObjects(Lure().getType(), &m_factory);
    m_DBLayer->loadObjects(Trip().getType(), &m_factory);
    m_DBLayer->loadObjects(Place().getType(), &m_factory);
    qDebug() << "finished loading";
}

void TrollingModel::importTrollingObject(TrollingObject* object)
{
    //Must not be already in trollingobjects
    if(!m_trollingobjects.contains(object))
    {
        TrollingObject* prev = getTrollingObject(object->getType(), object->getId());
        m_DBLayer->storeObject(object);
        if(prev != NULL)
        {
            reset(prev);
        }
        else
        {
            m_DBLayer->loadObjects(object->getType(), &m_factory, object->getId());
        }
    }
}

int TrollingModel::getMaxId(const QString& type)
{
    int maxId = 1;
    foreach(TrollingObject* object, m_trollingobjects)
    {
        if(object->getType() == type &&
           object->getId() > maxId)
        {
            maxId = object->getId();
        }
    }
    return maxId;
}

void TrollingModel::resetJournal()
{
    m_journal.resetJournal();
}

bool TrollingModel::inJournal(int id, const QString& type)
{
    return m_journal.inJournal(id, type);
}

int TrollingModel::getRevision(const QString& type)
{
    return m_DBLayer->getRevision(type);
}

void TrollingModel::setRevision(const QString& type, int revision)
{
    m_DBLayer->setRevision(revision, type);
}

Synchronizer* TrollingModel::getSynchronizer()
{
    return &m_synchronizer;
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

    if(getTrollingObject(Trip().getType(), id) == NULL)
        qDebug() << "will get NULL trip" << QString::number(id);

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
            // Do not cache nonpersistent objects.
            if (id >= 0) {
                m_fasterHash[id] = object;
            }

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

    reset(object);

    emit save();
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
    emit save();
}

void TrollingModel::reset(TrollingObject* p_object)
{
    if(p_object == NULL)
        return;

    ReloadingTrollingObjectFactory fakeFactory;
    fakeFactory.setTarget(p_object);
    m_DBLayer->loadObjects(p_object->getType(), &fakeFactory, p_object->getId());
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
