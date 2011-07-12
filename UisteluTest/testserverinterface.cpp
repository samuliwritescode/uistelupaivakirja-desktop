#include <QtCore/QString>
#include <QtTest/QtTest>
#include "singletons.h"
#include "trollingmodel.h"
#include "synchronizer.h"

class TestServerInterface : public QObject
{
    Q_OBJECT

public:
    TestServerInterface();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testGetting();
};

TestServerInterface::TestServerInterface()
{
}

void TestServerInterface::initTestCase()
{

}

void TestServerInterface::cleanupTestCase()
{
}

void TestServerInterface::testGetting()
{
    TrollingModel* model = Singletons::model();
    qDebug() << "number of trips in db" << QString::number(model->getTrips().count());
    Synchronizer* sync = model->getSynchronizer();
    QVERIFY2(true, "Failure");
}

#include "testserverinterface.moc"
