#include <QtGui/QApplication>
#include <QTextCodec>
#include <QSettings>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Samuli Penttila");
    QCoreApplication::setOrganizationDomain("fish.capeismi.fi");
    QCoreApplication::setApplicationName("Uistelupaivakirja");
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QSettings settings;
    QString programFolder = settings.value("ProgramFolder").toString();
    QString defaultFolder = QDir::homePath()+"/uistelu/";
    QDir dir;
    if(!dir.exists(programFolder))
    {
        if(QMessageBox::question(NULL,
                                 QObject::tr("Ilmoitus"),
                                 QObject::tr("Uistelupäiväkirjan tallennuskansio täytyy asettaa. Ehdotan kansioksi ")+
                                 defaultFolder+
                                 QObject::tr(" Onko tämä OK? Vastaamalla ei saat valita itse kansion."),
                                 QMessageBox::Yes,
                                 QMessageBox::No)
            == QMessageBox::Yes)
        {
            if(dir.mkpath(defaultFolder))
            {
                settings.setValue("ProgramFolder", defaultFolder);
            }
            else
            {
                QMessageBox::critical(NULL,
                                      QObject::tr("Ongelma"),
                                      QObject::tr("Ehdotettua kansiota ei voi luoda. Katso, että sinne on kirjoitusoikeudet tai valitse toinen kansio."));
                return 0;
            }
        }
        else
        {
            QFileDialog filedlg;
            filedlg.setOptions(QFileDialog::ShowDirsOnly);
            filedlg.setFileMode(QFileDialog::Directory);
            if(filedlg.exec())
            {
                QStringList folders = filedlg.selectedFiles();
                if(folders.size() == 1)
                {
                    QString folder = folders.at(0);
                    QDir dir;
                    if(!dir.exists(folder))
                    {
                        QMessageBox::critical(NULL,
                                              QObject::tr("Ongelma"),
                                              QObject::tr("Kansiota ")+
                                              folder+
                                              QObject::tr(" ei ole olemassa. Kansiota ei voi asettaa tallennuskansioksi"));
                    }
                    else
                    {
                        settings.setValue("ProgramFolder", folder);
                    }
                }
                else
                {
                    QMessageBox::critical(NULL,
                                          QObject::tr("Ongelma"),
                                          QObject::tr("Sain useamman kuin yhden kansion. Tämän ei pitäisi olla edes mahdollista."));
                    return 0;
                }
            }
            else
            {
                QMessageBox::critical(NULL,
                                      QObject::tr("Ongelma"),
                                      QObject::tr("Et valinnut mitään kansiota. Ilman tallennuskansiota ei ohjelmaa voi käyttää."));
                return 0;
            }
        }
    }

    MainWindow w;
    w.show();

    return a.exec();
}
