#ifndef TROLLINGMODEL_H
#define TROLLINGMODEL_H

#include <QObject>
#include <QDebug>
#include <QHash>
#include "trip.h"
#include "fish.h"
#include "lure.h"
#include "place.h"
#include "trollingobjectfactory.h"
#include "trollingobject.h"
#include "simpletrollingobjectfactory.h"
#include "dblayer.h"
#include "serverinterface.h"
#include "transactionjournal.h"
#include "synchronizer.h"

class TrollingModel : public QObject
{
    Q_OBJECT
public:
    explicit TrollingModel(QObject *parent = 0);
    virtual ~TrollingModel();
    Trip* getTrip(int id = -1);
    Lure* getLure(int id = -1);
    Place* getPlace(int id = -1);
    int commit(TrollingObject*);
    void remove(TrollingObject*);
    void reset(TrollingObject*);

    QMap<int, Trip*> getTrips();
    QMap<int, Lure*> getLures();
    QMap<int, Place*> getPlaces();
    void initialize();

    void importTrollingObject(TrollingObject*);

    QString importFile(TrollingObject*, const QString&);
    int getRevision(const QString&);
    void setRevision(const QString&, int);
    bool inJournal(int, const QString&);
    Synchronizer* getSynchronizer();

signals:

private:
    int getMaxId(const QString& type);

    SimpleTrollingObjectFactory m_factory;
    TrollingObject* getTrollingObject(const QString&, int);
    QList<TrollingObject*> m_trollingobjects;   
    QHash<int, TrollingObject*> m_fasterHash;
    DBLayer* m_DBLayer;
    QString m_filePath;
    TransactionJournal m_journal;
    Synchronizer m_synchronizer;
};

#endif // TROLLINGMODEL_H
