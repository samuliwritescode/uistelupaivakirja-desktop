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
    void downloadDone();
    void uploadDone();
    void error(const QString&);

public slots:
    void syncServer(const QString& folder);
    void upload();
    void download();

private:
    bool contains(QList<TrollingObject*> list, int id);
    ServerInterface m_server;

    void syncServerImpl(QMap<int, TrollingObject*>& objectsLocal, const QString& folder, const QString& type);

};

#endif // SYNCHRONIZER_H
