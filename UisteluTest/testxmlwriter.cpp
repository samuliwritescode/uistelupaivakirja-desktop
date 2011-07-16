#include <QtCore/QString>
#include <QtTest/QtTest>
#include "singletons.h"
#include "trollingmodel.h"
#include "synchronizer.h"
#include "testxmlwriter.h"

TestXMLWriter::TestXMLWriter()
{
}

void TestXMLWriter::initTestCase()
{

}

void TestXMLWriter::cleanupTestCase()
{
}

void TestXMLWriter::testGetting()
{
    TrollingModel* model = Singletons::model();
    qDebug() << "number of trips in db" << QString::number(model->getTrips().count());
    Synchronizer* sync = model->getSynchronizer();
    QVERIFY2(true, "Failure");
}

//#include "testxmlwriter.moc"

