#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QTableWidgetItem>
#include "settingsform.h"
#include "ui_settingsform.h"

SettingsForm::SettingsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);

    QSettings settings;
    ui->checkBoxUseSuggestions->setChecked(settings.value("use_suggestions").toBool());
    ui->checkBoxUseCustomFields->setChecked(settings.value("use_customfields").toBool());
    ui->customFields->setEnabled(settings.value("use_customfields").toBool());
    ui->checkBoxUseRouteLog->setChecked(settings.value("use_routelog").toBool());
    ui->lineEditDataFolder->setText(settings.value("ProgramFolder").toString());
    ui->mobileFolder->setText(settings.value("MobileFolder").toString());
    ui->lineEditCSS->setText(settings.value("TripReportCSS").toString());

    QString customField0 = settings.value("custom_field0").toString();
    QString customField1 = settings.value("custom_field1").toString();
    QString customField2 = settings.value("custom_field2").toString();
    QString customField3 = settings.value("custom_field3").toString();
    QString customField4 = settings.value("custom_field4").toString();

    ui->customFields->setItem(0,0, new QTableWidgetItem(customField0));
    ui->customFields->setItem(1,0, new QTableWidgetItem(customField1));
    ui->customFields->setItem(2,0, new QTableWidgetItem(customField2));
    ui->customFields->setItem(3,0, new QTableWidgetItem(customField3));
    ui->customFields->setItem(4,0, new QTableWidgetItem(customField4));
}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::on_openSaveFolder_clicked()
{
    QSettings settings;
    QFileDialog filedlg(this);
    filedlg.setOptions(QFileDialog::ShowDirsOnly);
    filedlg.setFileMode(QFileDialog::Directory);
    filedlg.setDirectory(settings.value("ProgramFolder").toString());
    if(filedlg.exec())
    {
        QStringList folders = filedlg.selectedFiles();
        if(folders.size() == 1)
        {
            QString folder = folders.at(0);
            QDir dir;
            if(!dir.exists(folder))
            {
                QMessageBox::critical(this, tr("Ongelma"), tr("Kansiota ")+folder+tr(" ei ole olemassa. Kansiota ei voi asettaa tallennuskansioksi"));
            }
            else
            {
                settings.setValue("ProgramFolder", folder);
                ui->lineEditDataFolder->setText(folder);
                QMessageBox::information(this, tr("Ilmoitus"), tr("Uusi tallennuskansio on asetettu. Ohjelman täytyy nyt sulkeutua. Muista kopioida vanhasta kansiosta sisältö, jos haluat säilyttää vanhat tietosi."));
                QCoreApplication::exit(0);
            }
        }
    }
}

void SettingsForm::on_openCSSFile_clicked()
{
    QSettings settings;
    QString filename = QFileDialog::getOpenFileName(this, tr("Avaa tyylitiedosto"), "", tr("Tyylitiedostot (*.css)"));
    if(QFile::exists(filename))
    {
        settings.setValue("TripReportCSS", filename);
        ui->lineEditCSS->setText(filename);
    }
}

void SettingsForm::on_checkBoxUseSuggestions_clicked(bool checked)
{
    QSettings settings;
    settings.setValue("use_suggestions", checked);
}

void SettingsForm::on_customFields_cellChanged(int row, int column)
{
    QSettings settings;
    QTableWidgetItem* item = ui->customFields->item(row, column);

    QString customField = "custom_field"+QString::number(row);
    settings.setValue(customField, item->text());
}

void SettingsForm::on_checkBoxUseCustomFields_clicked(bool checked)
{
    QSettings settings;
    settings.setValue("use_customfields", checked);

    ui->customFields->setEnabled(checked);
}

void SettingsForm::on_clearCSS_clicked()
{
    QSettings settings;
    settings.remove("TripReportCSS");
    ui->lineEditCSS->setText(QString());
}

void SettingsForm::on_checkBoxUseRouteLog_clicked(bool checked)
{
    QSettings settings;
    settings.setValue("use_routelog", checked);
}

void SettingsForm::on_openMobileFolder_clicked()
{
    QSettings settings;
    QFileDialog filedlg(this);
    filedlg.setOptions(QFileDialog::ShowDirsOnly);
    filedlg.setFileMode(QFileDialog::Directory);
    filedlg.setDirectory(settings.value("MobileFolder").toString());
    if(filedlg.exec())
    {
        QStringList folders = filedlg.selectedFiles();
        if(folders.size() == 1)
        {
            QString folder = folders.at(0);
            QDir dir(folder);
            if(dir.mkpath("uistelu/"))
            {
                settings.setValue("MobileFolder", folder);
                ui->mobileFolder->setText(folder);
            }
            else
            {
                 QMessageBox::critical(this, tr("Ongelma"), tr("Kansioon ")+folder+tr(" ei voi kirjoittaa. En voi käyttää hakemistoa."));
            }
        }
    }
}
