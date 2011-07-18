#ifndef DBLAYER_H
#define DBLAYER_H
#include <QObject>
#include <QString>
#include <QMutex>
#include <QDir>
#include "trollingobject.h"
#include "trollingobjectfactory.h"

class DBLayer: public QObject
{
    Q_OBJECT
public:
    explicit DBLayer(const QString& p_storePath, QObject *parent = 0);
    virtual ~DBLayer();

    bool storeObject(TrollingObject* p_object);
    bool loadObjects(const QString& p_type, TrollingObjectFactory* p_factory, int id = -1);
    bool removeObject(TrollingObject* p_object);
    QList<int> getIds(const QString& p_type);
    void setRevision(int, const QString&);
    int getRevision(const QString&);

signals:
    void storeSignal(int, QString);

private:
    QDir m_storeDir;
    QMutex m_mutex;
};

#endif // DBLAYER_H
