#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include "UisteluTest/testserverinterface.h"
#include "UisteluTest/testxmlwriter.h"

int main(int argc, char *argv[])
{
    int retval = 0;
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Samuli Penttila");
    QCoreApplication::setOrganizationDomain("fish.capeismi.fi");
    QCoreApplication::setApplicationName("Uistelupaivakirja");
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    retval += QTest::qExec(&TestServerInterface(), argc, argv);
   // retval += QTest::qExec(&TestXMLWriter(), argc, argv);
    return retval;
}
