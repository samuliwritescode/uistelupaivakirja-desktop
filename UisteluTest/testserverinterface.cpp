#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QSettings>
#include "singletons.h"
#include "trollingmodel.h"
#include "synchronizer.h"
#include "simpletrollingobjectfactory.h"
#include "dblayer.h"
#include <QFile>
#include <QDir>
#include <QTest>

#include "testserverinterface.h"

TestServerInterface::TestServerInterface()
{
}

void TestServerInterface::waitForSignal(QObject* object, const char* signal)
{
    QSignalSpy spy(object, signal);

    for(int loop=0; loop < 5*10; loop++)
    {
        QTest::qWait(200);
        if(spy.count() != 0)
            return;
    }

    QString signalname(signal);
    QString failmessage("timeout waiting for signal "+signalname);
    QFAIL(failmessage.toAscii());
}

void TestServerInterface::initTestCase()
{
    QCOMPARE(settings.value("ProgramFolder").toString().isEmpty(), false);
    QString path = settings.value("ProgramFolder").toString();

    QFile::remove(path+"/server/trip.xml");
    QFile::remove(path+"/database/trip.xml");
}

void TestServerInterface::cleanupTestCase()
{

}

void TestServerInterface::testSetting()
{
    Synchronizer* sync = Singletons::model()->getSynchronizer();
    connect(sync, SIGNAL(uploadDone()), this, SLOT(setDone()));
    sync->upload();
    waitForSignal(sync, SIGNAL(uploadDone()));
}

void TestServerInterface::setDone()
{

}

void TestServerInterface::testGetting()
{
    TrollingModel* model = Singletons::model();
    Synchronizer* sync = model->getSynchronizer();
    connect(sync, SIGNAL(downloadDone()), this, SLOT(getDone()));
    sync->download();
    waitForSignal(sync, SIGNAL(downloadDone()));
}

void TestServerInterface::getDone()
{
    QString path = settings.value("ProgramFolder").toString();
    DBLayer dbserver(path+"/server/");
    QList<TrollingObject*> list;
    SimpleTrollingObjectFactory factory;
    factory.setPushToCollection(&list);
    dbserver.loadObjects("trip", &factory);
    QVERIFY(list.count() == Singletons::model()->getTrips().count());
}


