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
#include "dblayer.h"

class TrollingModel : public QObject, public TrollingObjectFactory
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
    virtual TrollingObject* createTrollingObject(const QString& p_type);

signals:

public slots:

private:
    TrollingObject* getTrollingObject(const QString&, int);
    QList<TrollingObject*> m_trollingobjects;
    DBLayer* m_DBLayer;

};

#endif // TROLLINGMODEL_H
