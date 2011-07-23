#include <QSettings>
#include "trollingexception.h"
#include "synchronizer.h"
#include "dblayer.h"
#include "simpletrollingobjectfactory.h"
#include "singletons.h"
#include "alternative.h"

const int WAIT_BETWEEN_DOWNLOADS = 60000;
const int WAIT_AFTER_FAILURE = 5000;
const int WAIT_AFTER_MODIFY = 5000;

Synchronizer::Synchronizer(QObject *parent) :
    QObject(parent)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(run()));
    connect(&m_server, SIGNAL(checkoutDone(QString)), this, SIGNAL(downloadDone()));
    connect(&m_server, SIGNAL(checkoutDone(QString)), this, SLOT(checkoutDone(QString)));
    connect(&m_server, SIGNAL(commitDone()), this, SIGNAL(uploadDone()));
    connect(&m_server, SIGNAL(error(QString)), this, SLOT(handleError(QString)));
    connect(&m_server, SIGNAL(commitFile(QString,int)), this, SLOT(uploadFile(QString,int)));
    m_timer.setInterval(0);
    m_timer.setSingleShot(true);
    m_timer.start();
}

void Synchronizer::run()
{
    if(m_realms.count() > 0)
    {
        QMap<QString, QByteArray> realm = m_realms.last();
        qDebug() << "commiting";
        if(m_server.commit(realm.keys(), realm.values()))
        {
            m_realms.clear();
        }
        else
        {
            setTimerForFailure();
        }
    }
    else
    {
        download();
    }

    m_timer.setInterval(WAIT_BETWEEN_DOWNLOADS);
    m_timer.start();
}

void Synchronizer::setTimerForFailure()
{
    m_timer.stop();
    m_timer.setInterval(WAIT_AFTER_FAILURE);
    m_timer.start();
}

void Synchronizer::download()
{
    QStringList types;
    types << Trip().getType();
    types << Lure().getType();
    types << Place().getType();
    if(!m_server.checkout(types))
    {
        qDebug() << "busy";
    }
}

void Synchronizer::upload()
{
    QStringList types;
    types << Trip().getType();
    types << Lure().getType();
    types << Place().getType();
    QMap<QString, QByteArray> realm;

    foreach(QString doctype, types)
    {
        QSettings settings;
        QString path = settings.value("ProgramFolder").toString();
        QFile xml(path+"/database/"+doctype+".xml");
        if(xml.open(QIODevice::ReadOnly))
        {
            realm[doctype] = xml.readAll();
            xml.close();
        }
    }

    m_realms.append(realm);
    m_timer.stop();
    m_timer.setInterval(WAIT_AFTER_MODIFY);
    m_timer.start();
}

void Synchronizer::uploadFile(const QString& type, int revision)
{
    qDebug() << type << "new revision is" << revision;
    Singletons::model()->setRevision(type, revision);
}

void Synchronizer::checkoutDone(const QString& folder)
{
    populate(folder);
}

void Synchronizer::handleError(const QString& errorstr)
{
    //This I can recover.
    if(errorstr.contains("Cannot commit. Conflict with revision"))
    {
        qDebug() << "need getting fresh revision";
        setTimerForFailure();
    }else
    {
        emit error(errorstr);
    }
}

void Synchronizer::populate(const QString& folder)
{
    TrollingModel* model = Singletons::model();
    deleteAndClear();
    determineRevisions(folder);

    QStringList types;
    types << Trip().getType();
    types << Lure().getType();
    types << Place().getType();

    foreach(QString type, types)
    {
        int serverRevision = m_revisions[type];
        int localRevision = model->getRevision(type);
        if(serverRevision != 0 && serverRevision != localRevision)
        {
            determineChanges(getLocalObjects(type), getRemoteObjects(type, folder));
        }
    }

    emit downloadDone();
}

void Synchronizer::deleteAndClear()
{
    foreach(TrollingObject* o, m_added)
    {
        delete o;
    }

    foreach(TrollingObject* o, m_overwritten)
    {
        delete o;
    }

    foreach(TrollingObject* o, m_conflicting)
    {
        delete o;
    }

    m_added.clear();
    m_deleted.clear();
    m_overwritten.clear();
    m_conflicting.clear();
    m_revisions.clear();
}

QList<TrollingObject*> Synchronizer::getChanges(EChangeType type)
{
    QList<TrollingObject*> retval;

    switch(type)
    {
    case eAdded: retval += m_added; break;
    case eRemoved: retval += m_deleted; break;
    case eModified: retval += m_overwritten; break;
    case eConflicting: retval += m_conflicting; break;
    default: break;
    }

    return retval;
}

void Synchronizer::saveChanges(const QList<TrollingObject*>& changes)
{
    TrollingModel* model = Singletons::model();
    foreach(TrollingObject* o, m_added)
    {
        if(contains(changes, o->getId()))
        {
            qDebug() << "adding new";
            o->setId(-1);
            model->importTrollingObject(o);
        }
    }

    foreach(TrollingObject* o, m_conflicting)
    {
        if(contains(changes, o->getId()))
        {
            qDebug() << "adding conflicting";
            o->setId(-1);
            model->importTrollingObject(o);
        }
    }

    foreach(TrollingObject* o, m_deleted)
    {
        if(contains(changes, o->getId()))
        {
           qDebug() << "removing";
           model->remove(o);
        }
    }

    foreach(TrollingObject* o, m_overwritten)
    {
        if(contains(changes, o->getId()))
        {
            qDebug() << "replacing";
            model->importTrollingObject(o);
        }
    }

    foreach(QString type, m_revisions.keys())
    {
        int serverRevision = m_revisions[type];
        int oldRevision = model->getRevision(type);
        if(serverRevision != oldRevision)
        {
            model->setRevision(type, serverRevision);
        }
    }
    deleteAndClear();
}

QList<TrollingObject*> Synchronizer::getRemoteObjects(const QString& type, const QString& folder)
{
    QList<TrollingObject*> retval;
    DBLayer dbServer(folder);
    SimpleTrollingObjectFactory factory;
    factory.setPushToCollection(&retval);
    dbServer.loadObjects(type, &factory);

    return retval;
}

QMap<int, TrollingObject*> Synchronizer::getLocalObjects(const QString& type)
{
    QMap<int, TrollingObject*> retval;
    TrollingModel* model = Singletons::model();

    if(type == Trip().getType())
    {
        foreach(Trip* trip, model->getTrips())
        {
            retval[trip->getId()] = trip;
        }
    } else if(type == Lure().getType())
    {
        foreach(Lure* lure, model->getLures())
        {
            retval[lure->getId()] = lure;
        }
    } else if(type == Place().getType())
    {
        foreach(Place* place, model->getPlaces())
        {
            retval[place->getId()] = place;
        }
    }

    return retval;
}

void Synchronizer::determineRevisions(const QString& remoteFolder)
{
    DBLayer dbServer(remoteFolder);
    m_revisions[Trip().getType()] = dbServer.getRevision(Trip().getType());
    m_revisions[Place().getType()] = dbServer.getRevision(Place().getType());
    m_revisions[Lure().getType()] = dbServer.getRevision(Lure().getType());
}

void Synchronizer::determineChanges(QMap<int, TrollingObject*> objectsLocal, QList<TrollingObject*>  objectsRemote)
{
    TrollingModel* model = Singletons::model();

    //Check what are removed
    foreach(int objid, objectsLocal.keys())
    {
        if(!contains(objectsRemote, objid))
        {
            m_deleted.append(objectsLocal[objid]);
        }
    }

    //Check what are modified or new
    foreach(TrollingObject* o, objectsRemote)
    {
        if(objectsLocal.contains(o->getId()))
        {
            TrollingObject* trip = objectsLocal[o->getId()];
            if(!(*trip == *o))
            {
                if(model->inJournal(trip->getId(), trip->getType()) || trip->isUnsaved())
                {
                    m_conflicting.append(o);
                    continue;
                }
                else
                {
                    m_overwritten.append(o);
                    continue;
                }
            }
        }
        else
        {
            m_added.append(o);
            continue;
        }

        delete o;
    }
}

bool Synchronizer::contains(QList<TrollingObject*> list, int id)
{
    foreach(TrollingObject* object, list)
    {
        if(object->getId() == id)
        {
            return true;
        }
    }

    return false;
}

int Synchronizer::syncMobile()
{
    TrollingModel* model = Singletons::model();
    int syncedobjects = 0;
    QSettings settings;
    QString filePath = settings.value("ProgramFolder").toString();
    QString memCard = settings.value("MobileFolder").toString();
    if(!memCard.isEmpty())
    {
        if(!QDir().exists(memCard))
        {
            throw TrollingException(tr("Muistikorttia ei löydy. Tarkista puhelimen kytkentä."));
        }

        if(!QDir().exists(memCard+"/uistelu/"))
        {
            QDir().mkpath(memCard+"/uistelu/");
        }

        QFile::remove(memCard+"/uistelu/lure.xml");
        QFile::remove(memCard+"/uistelu/place.xml");
        QFile::remove(memCard+"/uistelu/spinneritems.xml");
        if(!QFile::copy(filePath+"/database/lure.xml", memCard+"/uistelu/lure.xml"))
        {
            throw TrollingException(tr("En kykene kopioimaan viehekantaa muistikortille. Tarkista ettei kortti ole kirjoitussuojattu"));
        }

        if(!QFile::copy(filePath+"/database/place.xml", memCard+"/uistelu/place.xml"))
        {
             throw TrollingException(tr("En kykene kopioimaan paikkakantaa muistikortille. Tarkista ettei kortti ole kirjoitussuojattu"));
        }

        DBLayer dblayerMobile(memCard+"/uistelu/");
        QList<TrollingObject*> mobiletrollingobjects;
        SimpleTrollingObjectFactory factory;
        factory.setPushToCollection(&mobiletrollingobjects);
        dblayerMobile.loadObjects(Trip().getType(), &factory);

        //int maxId = generateId(model->getTrips());

        foreach(TrollingObject* object, mobiletrollingobjects)
        {
            syncedobjects++;
            //qDebug() << "setting id to mobile trip" << maxId;
            object->setId(-1);

            model->importTrollingObject(object);
            delete object;
            //maxId++;
        }

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

    return syncedobjects;
}
