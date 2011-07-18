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

    enum EChangeType{eUnknown, eAdded, eRemoved, eModified, eConflicting};

    /**
      List returned from getChanges() must not be stored
      by caller. List is only valid immediately after
      calling getChanges() and before calling saveChanges().
      Ownership of TrollingObject-pointers is belongs to
      Synchronizer.

      Otherwise the behaviour is undefined and galaxies could
      explode and Keinonen would descend to punish everybody.
      */
    QList<TrollingObject*> getChanges(EChangeType);
    void saveChanges(const QList<TrollingObject*>&);
    int syncMobile();


signals:
    void downloadDone();
    void uploadDone();
    void error(const QString&);

public slots:
    void upload();
    void download();

private slots:
    void populate(const QString&);

private:
    void deleteAndClear();
    QMap<int, TrollingObject*> getLocalObjects(const QString&);
    QList<TrollingObject*> getRemoteObjects(const QString&, const QString&);
    bool contains(QList<TrollingObject*> list, int id);
    ServerInterface m_server;
    void determineChanges(QMap<int, TrollingObject*> objectsLocal, QList<TrollingObject*> objectsRemote);
    void determineRevisions(const QString& remoteFolder);
    QList<TrollingObject*> m_deleted;
    QList<TrollingObject*> m_added;
    QList<TrollingObject*> m_overwritten;
    QList<TrollingObject*> m_conflicting;
    QMap<QString, int> m_revisions;

};

#endif // SYNCHRONIZER_H
