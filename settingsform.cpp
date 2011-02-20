#include <QSettings>
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

}

void SettingsForm::on_checkBoxUseCustomFields_clicked(bool checked)
{
    QSettings settings;
    settings.setValue("use_customfields", checked);

    ui->customFields->setEnabled(checked);
}
