#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H

#include <QObject>
#include "trollingobject.h"
#include "serverinterface.h"
class Trip;

class Synchronizer : public QObject
{
    Q_OBJECT
public:
    explicit Synchronizer(QObject *parent = 0);
    int syncMobile();

signals:

public slots:
    void syncServer(const QString& folder);
    void upload();

private:
    int generateId(const QMap<int, Trip*>& trips);
    bool contains(QList<TrollingObject*> list, int id);
    ServerInterface m_server;

};

#endif // SYNCHRONIZER_H
