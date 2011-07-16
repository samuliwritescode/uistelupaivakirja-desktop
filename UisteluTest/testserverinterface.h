#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QSettings>

class TestServerInterface : public QObject
{
    Q_OBJECT

public:
    TestServerInterface();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testSetting();
    void testGetting();

public slots:
    void getDone();
    void setDone();

private:
    void waitForSignal(QObject*, const char*);

private:
    QSettings settings;
};
