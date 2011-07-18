#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Singletons::tripController()->setMessageDisplay(this);
    Singletons::placeController()->setMessageDisplay(this);
    Singletons::lureController()->setMessageDisplay(this);
    Singletons::syncController()->setMessageDisplay(this);
    ui->statsTab->layout()->addWidget(new StatisticsForm());
    ui->tripsTab->layout()->addWidget(new TripForm());
    ui->luresTab->layout()->addWidget(new LureForm());
    ui->placesTab->layout()->addWidget(new PlaceForm());
    ui->settingsTab->layout()->addWidget(new SettingsForm());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showNotificationMessage(const QString& notify)
{
    QMessageBox::information(this, tr("Ilmoitus"), notify);
}

void MainWindow::showErrorMessage(const QString& error)
{
    QMessageBox::critical(this, tr("Ongelma"), error);
}

bool MainWindow::showConfirmationMessage(const QString& question)
{
    if (QMessageBox::question(this, tr("Kysymys"), question, QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        return true;
    return false;
}

MessageDisplay::EChoices MainWindow::showChoiceMessage(const QString& message)
{
    QMessageBox box;
    box.setIcon(QMessageBox::Question);
    box.setText(message);
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Cancel);
    int res = box.exec();
    if(res == QMessageBox::Yes)
        return MessageDisplay::eYes;
    else if(res == QMessageBox::No)
        return MessageDisplay::eNo;
    else
        return MessageDisplay::eCancel;
}

void MainWindow::showStatusMessage(const QString& message, bool stay)
{
    if(stay)
        ui->statusBar->showMessage(message);
    else
        ui->statusBar->showMessage(message, 10000);

}
