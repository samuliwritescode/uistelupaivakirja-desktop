#include <QtGui/QApplication>
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Samuli Penttila");
    QCoreApplication::setOrganizationDomain("fish.capeismi.fi");
    QCoreApplication::setApplicationName("Uistelupaivakirja");
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    MainWindow w;
    w.show();

    return a.exec();
}
