#ifndef TROLLINGMODEL_H
#define TROLLINGMODEL_H

#include <QObject>
#include <QDebug>
#include <QHash>
#include "trip.h"
#include "fish.h"
#include "lure.h"
#include "site.h"
#include "species.h"
#include "trollingobject.h"
#include "dblayer.h"

class TrollingModel : public QObject
{
    Q_OBJECT
public:
    explicit TrollingModel(QObject *parent = 0);
    virtual ~TrollingModel();
    Trip* getTrip(int id = -1);
    Lure* getLure(int id = -1);
    Site* getSite(int id = -1);
    Species* getSpecies(int id = -1);
    int commit(TrollingObject*);

    QMap<int, Trip*> getTrips();

signals:

public slots:

private:
    QList<TrollingObject*> m_trollingobjects;
    DBLayer* m_DBLayer;

};

#endif // TROLLINGMODEL_H
