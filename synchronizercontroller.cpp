#include <QDebug>
#include <QTimer>
#include "singletons.h"
#include "synchronizercontroller.h"
#include "revisiondialog.h"

SynchronizerController::SynchronizerController(QObject *parent) :
    Controller(parent)
{
    m_sync = Singletons::model()->getSynchronizer();
    m_timer.setSingleShot(true);
    m_timer.setInterval(1000);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(checkUpdates()));
    connect(m_sync, SIGNAL(downloadDone()), this, SLOT(downloadDone()));
    connect(m_sync, SIGNAL(uploadDone()), this, SLOT(uploadDone()));
    connect(m_sync, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(Singletons::model(), SIGNAL(save()), this, SLOT(uploadChanges()));
    m_timer.start();
}

void SynchronizerController::checkUpdates()
{
    qDebug() << "timer fire";
    m_sync->download();
}

void SynchronizerController::downloadDone()
{
    QList<TrollingObject*> changesAdded = m_sync->getChanges(Synchronizer::eAdded);
    QList<TrollingObject*> changesModified = m_sync->getChanges(Synchronizer::eModified);
    QList<TrollingObject*> changesRemoved = m_sync->getChanges(Synchronizer::eRemoved);
    QList<TrollingObject*> changesConflicting = m_sync->getChanges(Synchronizer::eConflicting);

    int totalChanges = changesAdded.size() + changesConflicting.size() + changesModified.size() + changesRemoved.size();

    if(totalChanges == 0)
    {
        m_sync->saveChanges(changesAdded);
    }
    else
    {
        RevisionDialog dlg;

        dlg.setAdded(trollingObjectToStringList(changesAdded));
        dlg.setModified(trollingObjectToStringList(changesModified));
        dlg.setConflicting(trollingObjectToStringList(changesConflicting));
        dlg.setRemoved(trollingObjectToStringList(changesRemoved));
        dlg.setModal(true);
        if(dlg.exec() == QDialog::Accepted)
        {
            QList<TrollingObject*> changes;
            if(!dlg.isDontSave())
            {
                changes += changesAdded;
                changes += changesConflicting;
                changes += changesModified;
                changes += changesRemoved;
            }

            m_sync->saveChanges(changes);
            sendNotificationToObservers(Controller::eTripListUpdated);
            sendNotificationToObservers(Controller::eLureListUpdated);
            sendNotificationToObservers(Controller::ePlaceListUpdated);
        }
    }

    m_timer.start();
}

void SynchronizerController::uploadChanges()
{
    m_sync->upload();
}

void SynchronizerController::uploadDone()
{
    showNotificationMessage(tr("Lähetetty"));
}

void SynchronizerController::error(const QString& error)
{
    showErrorMessage(tr("Palvelinsynkronointi antoi virheen: ")+error);
    m_timer.start();
}

QStringList SynchronizerController::trollingObjectToStringList(const QList<TrollingObject*>& objects)
{
    QStringList retval;
    foreach(TrollingObject* o, objects)
    {
        QString label = getTypeName(o->getType());
        label += ", ";
        label += o->toString();
        retval.append(label);
    }

    return retval;
}

QString SynchronizerController::getTypeName(const QString& type)
{
    if(type == Trip().getType())
    {
        return tr("reissu");
    }
    else if(type == Place().getType())
    {
        return tr("kalapaikka");
    }
    else if(type == Lure().getType())
    {
        return tr("viehe");
    }

    return type;
}
