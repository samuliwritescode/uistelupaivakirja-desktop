#include <QSettings>
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

}

void SettingsForm::on_openCSSFile_clicked()
{

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
