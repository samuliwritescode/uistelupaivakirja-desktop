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
    //void test01Basic_data();
    void test01Basic();
    void test01MultipleGets();

public slots:
    void getDone();
    void setDone();
    void error(const QString&);
    void expectError(const QString&);

private:
    void waitForSignal(QObject*, const char*);
    void doSet(bool expectError, bool isData);
    void clearServer();
    void clearDatabase();

private:
    QSettings settings;
    QString path;
};
