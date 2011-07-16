#include <QtCore/QString>
#include <QtTest/QtTest>
#include "singletons.h"
#include "trollingmodel.h"
#include "synchronizer.h"

class TestXMLWriter : public QObject
{
    Q_OBJECT

public:
    TestXMLWriter();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testGetting();
};
