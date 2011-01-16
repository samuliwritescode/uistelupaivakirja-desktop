#ifndef TROLLINGMODEL_H
#define TROLLINGMODEL_H

#include <QObject>
#include <QDebug>
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

signals:

public slots:

private:
    QMap<int, Trip*> m_trips;
    DBLayer* m_DBLayer;

};

#endif // TROLLINGMODEL_H
