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
    DBLayer dbServer(folder);

    QList<TrollingObject*> objects;
    SimpleTrollingObjectFactory factory;
    factory.setPushToCollection(&objects);
    dbServer.loadObjects(Trip().getType(), &factory);

    int serverRevision = dbServer.getRevision(Trip().getType());
    int localRevision = model->getRevision(Trip().getType());
    if(serverRevision != localRevision)
    {
        qDebug() << "now ready to merge";
        QMap<int, Trip*> trips = model->getTrips();

        //Check what are removed
        foreach(int tripid, trips.keys())
        {
            if(!contains(objects, tripid))
            {
                qDebug() << "remove trip" << QString::number(tripid);
                model->remove(trips[tripid]);
            }
        }

        //Check what are modified or new
        foreach(TrollingObject* o, objects)
        {
            if(trips.contains(o->getId()))
            {
                Trip* trip = trips[o->getId()];
                if(!(*trip == *o))
                {
                    qDebug() << "trip are different: "+QString::number(trip->getId());
                    if(model->inJournal(trip->getId(), trip->getType()) || trip->isUnsaved())
                    {
                        qDebug() << "unsynched trip. need to create new" << QString::number(generateId(trips));
                        o->setId(generateId(trips));
                        model->importTrollingObject(o);
                    }
                    else
                    {
                        qDebug() << "modified trip. override from server";
                        model->importTrollingObject(o);
                    }
                }
            }
            else
            {
                qDebug() << "create new trip";
                o->setId(generateId(trips));
                model->importTrollingObject(o);
            }
        }

        model->setRevision(Trip().getType(), serverRevision);
    }
    else
    {
        qDebug() << "No need to merge. Revisions are match";
    }

    foreach(TrollingObject* object, objects)
    {
        delete object;
    }
    emit downloadDone();
}

int Synchronizer::generateId(const QMap<int, Trip*>& trips)
{
    if(trips.keys().size() == 0)
        return 1;
    else
        return trips.keys().last()+1;
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

        int maxId = generateId(model->getTrips());

        foreach(TrollingObject* object, mobiletrollingobjects)
        {
            syncedobjects++;
            qDebug() << "setting id to mobile trip" << maxId;
            object->setId(maxId);

            model->importTrollingObject(object);
            delete object;
            maxId++;
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
