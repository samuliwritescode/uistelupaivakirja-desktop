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
    path = settings.value("ProgramFolder").toString();
}

void TestServerInterface::cleanupTestCase()
{

}

void TestServerInterface::clearServer()
{
    QFile::remove(path+"/server/trip.xml");
    QFile::remove(path+"/server/place.xml");
    QFile::remove(path+"/server/lure.xml");

}

void TestServerInterface::clearDatabase()
{
    QFile::remove(path+"/database/trip.xml");
    QFile::remove(path+"/database/place.xml");
    QFile::remove(path+"/database/lure.xml");
}

void TestServerInterface::doSet(bool expectError, bool isData)
{
    if(isData == true)
    {
        DBLayer dblayer(path+"/database/");
        dblayer.storeObject(new Trip());
    }

    Synchronizer* sync = Singletons::model()->getSynchronizer();
    connect(sync, SIGNAL(uploadDone()), this, SLOT(setDone()));

    if(expectError == true)
    {
        connect(sync, SIGNAL(error(const QString&)), this, SLOT(expectError(const QString&)));
        sync->upload();
        waitForSignal(sync, SIGNAL(error(const QString&)));
        disconnect(sync, SIGNAL(error(const QString&)), this, SLOT(expectError(const QString&)));
    }
    else
    {
        connect(sync, SIGNAL(error(const QString&)), this, SLOT(error(const QString&)));
        sync->upload();
        waitForSignal(sync, SIGNAL(uploadDone()));
        disconnect(sync, SIGNAL(error(const QString&)), this, SLOT(error(const QString&)));
    }
}

/*
void TestServerInterface::test01Basic_data()
{
    QTest::addColumn<QString>("isData");
    QTest::addColumn<QString>("expectError");

    QTest::newRow("null file out of synch") << "false" << "true";
    //QTest::newRow("data file out of synch") << "true" << "true";
}*/

void TestServerInterface::test01Basic()
{
    clearServer();
    clearDatabase();

    Synchronizer* sync = Singletons::model()->getSynchronizer();
    connect(sync, SIGNAL(downloadDone()), this, SLOT(getDone()));
    connect(sync, SIGNAL(uploadDone()), this, SLOT(setDone()));
    connect(sync, SIGNAL(error(const QString&)), this, SLOT(error(const QString&)));

    sync->download();
    waitForSignal(sync, SIGNAL(downloadDone()));

    int revBefore = Singletons::model()->getRevision(Lure().getType());
    int countBefore = Singletons::model()->getTrips().count() +
                      Singletons::model()->getLures().count() +
                      Singletons::model()->getPlaces().count();

    Trip* trip = Singletons::model()->getTrip();
    Lure* lure = Singletons::model()->getLure();
    Place* place = Singletons::model()->getPlace();

    Singletons::model()->commit(lure);
    Singletons::model()->commit(place);

    trip->setPlace(place);
    trip->setDate(QDate::currentDate());
    trip->setTime(QTime::currentTime(), QTime::currentTime());

    Singletons::model()->commit(trip);

    int countAfter = Singletons::model()->getTrips().count() +
                      Singletons::model()->getLures().count() +
                      Singletons::model()->getPlaces().count();

    QCOMPARE(countBefore+3, countAfter);

    sync->upload();
    waitForSignal(sync, SIGNAL(uploadDone()));

    sync->download();
    waitForSignal(sync, SIGNAL(downloadDone()));

    int revAfter = Singletons::model()->getRevision(Lure().getType());
    countAfter = Singletons::model()->getTrips().count() +
                          Singletons::model()->getLures().count() +
                          Singletons::model()->getPlaces().count();


    QVERIFY(revBefore < revAfter);
    QCOMPARE(countBefore+3, countAfter);
}

void TestServerInterface::test01MultipleGets()
{
    Synchronizer* sync = Singletons::model()->getSynchronizer();
    connect(sync, SIGNAL(downloadDone()), this, SLOT(getDone()));
    connect(sync, SIGNAL(uploadDone()), this, SLOT(setDone()));
    connect(sync, SIGNAL(error(const QString&)), this, SLOT(error(const QString&)));

    sync->download();
    waitForSignal(sync, SIGNAL(downloadDone()));
    int tripBefore = Singletons::model()->getTrips().count();

    for(int loop=0; loop < 10; loop++)
    {
        sync->download();
        waitForSignal(sync, SIGNAL(downloadDone()));
        QCOMPARE(tripBefore, Singletons::model()->getTrips().count());
    }

}

void TestServerInterface::setDone()
{
    QVERIFY(QFile::exists(path+"/database/trip.xml"));
    QVERIFY(QFile::exists(path+"/database/lure.xml"));
    QVERIFY(QFile::exists(path+"/database/place.xml"));
}

void TestServerInterface::expectError(const QString& message)
{
}

void TestServerInterface::error(const QString& message)
{
    qWarning() << message;
    QFAIL("got error while not expected");
}

void TestServerInterface::getDone()
{
    QVERIFY(QFile::exists(path+"/server/trip.xml"));
    QVERIFY(QFile::exists(path+"/server/place.xml"));
    QVERIFY(QFile::exists(path+"/server/lure.xml"));
    DBLayer dbserver(path+"/server/");
    DBLayer dblocal(path+"/database/");

    QList<TrollingObject*> listTrips;
    QList<TrollingObject*> listLures;
    QList<TrollingObject*> listPlaces;

    QList<TrollingObject*> listTripsLocal;
    QList<TrollingObject*> listLuresLocal;
    QList<TrollingObject*> listPlacesLocal;

    SimpleTrollingObjectFactory factory;

    factory.setPushToCollection(&listTrips);
    dbserver.loadObjects("trip", &factory);

    factory.setPushToCollection(&listPlaces);
    dbserver.loadObjects("place", &factory);

    factory.setPushToCollection(&listLures);
    dbserver.loadObjects("lure", &factory);

    factory.setPushToCollection(&listTripsLocal);
    dblocal.loadObjects("trip", &factory);

    factory.setPushToCollection(&listPlacesLocal);
    dblocal.loadObjects("place", &factory);

    factory.setPushToCollection(&listLuresLocal);
    dblocal.loadObjects("lure", &factory);

    QVERIFY(listTrips.count() == Singletons::model()->getTrips().count());
    QVERIFY(listPlaces.count() == Singletons::model()->getPlaces().count());
    QVERIFY(listLures.count() == Singletons::model()->getLures().count());

    QVERIFY(listTrips.count() == listTripsLocal.count());
    QVERIFY(listPlaces.count() == listPlacesLocal.count());
    QVERIFY(listLures.count() == listLuresLocal.count());
}


