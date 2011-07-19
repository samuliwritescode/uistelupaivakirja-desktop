#ifndef SYNCHRONIZERCONTROLLER_H
#define SYNCHRONIZERCONTROLLER_H

#include <QTimer>
#include "controller.h"
#include "synchronizer.h"

class SynchronizerController : public Controller
{
    Q_OBJECT
public:
    explicit SynchronizerController(QObject *parent = 0);  

signals:

public slots:
    void checkUpdates();
    void uploadChanges();
    void downloadDone();
    void uploadDone();
    void error(const QString&);

private:
    QString getTypeName(const QString&);
    QStringList trollingObjectToStringList(const QList<TrollingObject*>& objects);

    Synchronizer* m_sync;

};

#endif // SYNCHRONIZERCONTROLLER_H
