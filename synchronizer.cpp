#include <QSettings>
#include "trollingexception.h"
#include "synchronizer.h"
#include "dblayer.h"
#include "simpletrollingobjectfactory.h"
#include "singletons.h"
#include "alternative.h"

Synchronizer::Synchronizer(QObject *parent) :
    QObject(parent)
{
    connect(&m_server, SIGNAL(checkoutDone(QString)), this, SLOT(syncServer(QString)));
    connect(&m_server, SIGNAL(commitDone()), this, SIGNAL(uploadDone()));
    connect(&m_server, SIGNAL(error(const QString&)), this, SIGNAL(error(const QString&)));
}

void Synchronizer::download()
{
    m_server.checkout();
}

void Synchronizer::upload()
{
    m_server.commit();
}

void Synchronizer::syncServer(const QString& folder)
{
    TrollingModel* model = Singletons::model();
    QMap<int, TrollingObject*> objects;

    foreach(Trip* trip, model->getTrips())
        objects[trip->getId()] = trip;

    syncServerImpl(objects, folder, Trip().getType());

    objects.clear();
    foreach(Lure* lure, model->getLures())
        objects[lure->getId()] = lure;

    syncServerImpl(objects, folder, Lure().getType());

    objects.clear();
    foreach(Place* place, model->getPlaces())
        objects[place->getId()] = place;

    syncServerImpl(objects, folder, Place().getType());

    emit downloadDone();
}

void Synchronizer::syncServerImpl(QMap<int, TrollingObject*>& objectsLocal, const QString& folder, const QString& type)
{
    TrollingModel* model = Singletons::model();
    DBLayer dbServer(folder);

    QList<TrollingObject*> objectsRemote;
    SimpleTrollingObjectFactory factory;
    factory.setPushToCollection(&objectsRemote);
    dbServer.loadObjects(type, &factory);

    int serverRevision = dbServer.getRevision(type);
    int localRevision = model->getRevision(type);
    if(serverRevision != localRevision || true)
    {
        qDebug() << "now ready to merge" << type;

        //Check what are removed
        foreach(int objid, objectsLocal.keys())
        {
            if(!contains(objectsRemote, objid))
            {
                qDebug() << "remove" << type << QString::number(objid);
                model->remove(objectsLocal[objid]);
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
                    qDebug() << type << "are different: "+QString::number(trip->getId());
                    qDebug() << QString::number(trip->getId()) << "id" << QString::number(o->getId());
                    qDebug() << trip->getType() << "type" << o->getType();
                    if(model->inJournal(trip->getId(), trip->getType()) || trip->isUnsaved())
                    {
                        qDebug() << "unsynched trip. need to create new";
                        if(model->inJournal(trip->getId(), trip->getType()))
                            qDebug() << "is in journal";
                        o->setId(-1);
                        model->importTrollingObject(o);
                    }
                    else
                    {
                        qDebug() << "modified" << type << ". override from server";
                        model->importTrollingObject(o);
                    }
                }
            }
            else
            {
                qDebug() << "create new" << type << "old id was" << QString::number(o->getId());
                o->setId(-1);
                model->importTrollingObject(o);
            }
        }

        model->setRevision(type, serverRevision);
    }
    else
    {
        qDebug() << "No need to merge. Revisions are match";
    }

    foreach(TrollingObject* object, objectsRemote)
    {
        delete object;
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
