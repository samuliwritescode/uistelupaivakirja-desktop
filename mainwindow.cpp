#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lureitem.h"
#include "singletons.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_tripController(NULL)
{
    m_tripController = Singletons::tripController();
    ui->setupUi(this);
    ui->water_temp->setValidator(new QDoubleValidator(this));
    ui->air_temp_start->setValidator(new QDoubleValidator(this));
    ui->air_temp_end->setValidator(new QDoubleValidator(this));
    ui->weight->setValidator(new QDoubleValidator(this));
    ui->length->setValidator(new QDoubleValidator(this));
    ui->spotdepth->setValidator(new QDoubleValidator(this));

//    ui->dateEdit->setDate(QDate::currentDate());
//    ui->place->insertItems(0, Singletons::placeController()->getPlaces());



  //  ui->species->insertItem(0, "Hauki");
  //  ui->species->insertItem(0, "Ahven");
  //  ui->species->insertItem(0, "Kuha");


   /* for(int loop=0; loop < 10; loop++)
    {
        LureItem* draggable = new LureItem("HK varma");
        LureItem* poi = new LureItem("12:22 hauki 13.3-1");

        QComboBox* kalat = new QComboBox();
        kalat->insertItem(0, "hauki");
        kalat->insertItem(0, "ahven");
        draggable->setAcceptDrops(true);*/
      /*  ui->catchGrid->addWidget(kalat, loop, 0);
        ui->catchGrid->addWidget(new QLineEdit("6kg"), loop, 1);
        ui->catchGrid->addWidget(new QLineEdit("85cm"), loop, 2);
        ui->catchGrid->addWidget(new QLineEdit("22C"), loop, 3);
        ui->catchGrid->addWidget(new QLineEdit("5m"), loop, 4);
        ui->catchGrid->addWidget(draggable, loop, 5);
        ui->catchGrid->addWidget(poi, loop, 6);*/
    //}

    //ui->lureList->setIconSize(QSize(100,50));
   /* for(int loop=0; loop < 100; loop++)
    {
        QListWidgetItem* item = new QListWidgetItem("HK varma taimen 9 cm venäjän lippu"+QString::number(loop));
        item->setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        //item->setIcon(QIcon(":/msnlogo.jpg"));
        ui->lureList->addItem(item);
    }*/

    observerEvent(Controller::eTripUpdated);
    observerEvent(Controller::eTripListUpdated);
    observerEvent(Controller::eFishListUpdated);

    connect(m_tripController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::observerEvent(int type)
{
    if(type == Controller::eTripUpdated)
    {
        ui->dateEdit->setDate(m_tripController->getDateValue(eTripDate));

        ui->startDial->setValue(m_tripController->getIntValue(eStartTime));
        ui->timeStartLbl->setText(tr("aloitus klo ")+QString::number(m_tripController->getIntValue(eStartTime)));

        ui->endDial->setValue(m_tripController->getIntValue(eEndTime));
        ui->timeEndLbl->setText(tr("lopetus klo ")+QString::number(m_tripController->getIntValue(eEndTime)));

        ui->weather_clear->setChecked(m_tripController->getBooleanValue(eWeatherClear));
        ui->weather_halfclear->setChecked(m_tripController->getBooleanValue(eWeatherHalfClear));
        ui->weather_overcast->setChecked(m_tripController->getBooleanValue(eWeatherOvercast));
        ui->weather_rain->setChecked(m_tripController->getBooleanValue(eWeatherRain));
        ui->weather_fog->setChecked(m_tripController->getBooleanValue(eWeatherFog));

        ui->wind_calm->setChecked(m_tripController->getBooleanValue(eWindCalm));
        ui->wind_faint->setChecked(m_tripController->getBooleanValue(eWindFaint));
        ui->wind_moderate->setChecked(m_tripController->getBooleanValue(eWindModerate));
        ui->wind_brisk->setChecked(m_tripController->getBooleanValue(eWindBrisk));
        ui->wind_hard->setChecked(m_tripController->getBooleanValue(eWindHard));

        if(m_tripController->getDoubleValue(eLength) != 0)
            ui->length->setText(QString::number(m_tripController->getDoubleValue(eLength)));
        else
            ui->length->clear();

        if(m_tripController->getDoubleValue(eWeight) != 0)
            ui->weight->setText(QString::number(m_tripController->getDoubleValue(eWeight)));
        else
            ui->weight->clear();

        if(m_tripController->getDoubleValue(eSpotDepth) != 0)
            ui->spotdepth->setText(QString::number(m_tripController->getDoubleValue(eSpotDepth)));
        else
            ui->spotdepth->clear();

    }
    else if(type == Controller::eTripListUpdated)
    {
        ui->trip_list->clear();
        QMap<QString, int> trips = m_tripController->getTripList();
        for(QMap<QString, int>::iterator iter = trips.begin(); iter != trips.end(); iter++)
        {
            QListWidgetItem* item = new QListWidgetItem(iter.key(), ui->trip_list, iter.value());
            ui->trip_list->insertItem(0, item);
        }
    }
    else if (type == Controller::eFishListUpdated)
    {
        qDebug() << "update fish list";
        ui->fish_list->clear();
        ui->fish_list->clearContents();
        ui->fish_list->setRowCount(0);
        QStringList headers;
        headers << tr("laji") << tr("paino") << tr("pituus") << tr("syvyys");
        ui->fish_list->setHorizontalHeaderLabels(headers);
        QList<QMap<QString, QString> > fishes = m_tripController->getFishList();
        for(int loop=0; loop < fishes.size(); loop++)
        {
            QMap<QString, QString> props = fishes.at(loop);
            ui->fish_list->insertRow(loop);
            ui->fish_list->setItem(loop, 0, new QTableWidgetItem(props["specie"], loop));
            ui->fish_list->setItem(loop, 1, new QTableWidgetItem(props["weight"], loop));
            ui->fish_list->setItem(loop, 2, new QTableWidgetItem(props["length"], loop));
            ui->fish_list->setItem(loop, 3, new QTableWidgetItem(props["depth"], loop));
            qDebug() << "weight" << props["weight"].toDouble();
        }
    }
}

void MainWindow::on_dateEdit_dateChanged(QDate date)
{    
    m_tripController->dateEvent(eTripDate, ui->dateEdit->date());
}


void MainWindow::on_place_currentIndexChanged(QString place)
{
    m_tripController->textEvent(ePlaceText, place);
}

void MainWindow::on_trip_save_clicked()
{
    m_tripController->buttonEvent(eSaveTrip);
}

void MainWindow::on_water_temp_textChanged(QString temp)
{
    m_tripController->textEvent(eWaterTemp, temp);
}

void MainWindow::on_misc_textChanged()
{
    m_tripController->textEvent(eMiscText, ui->misc->toPlainText());
}

void MainWindow::on_weather_clear_clicked(bool checked)
{
    m_tripController->booleanEvent(eWeatherClear, checked);
}

void MainWindow::on_weather_halfclear_clicked(bool checked)
{
    m_tripController->booleanEvent(eWeatherHalfClear, checked);
}

void MainWindow::on_weather_overcast_clicked(bool checked)
{
    m_tripController->booleanEvent(eWeatherOvercast, checked);
}

void MainWindow::on_weather_rain_clicked(bool checked)
{
    m_tripController->booleanEvent(eWeatherRain, checked);
}

void MainWindow::on_weather_fog_clicked(bool checked)
{
    m_tripController->booleanEvent(eWeatherFog, checked);
}

void MainWindow::on_wind_calm_clicked(bool checked)
{
    m_tripController->booleanEvent(eWindCalm, checked);
}

void MainWindow::on_wind_faint_clicked(bool checked)
{
    m_tripController->booleanEvent(eWindFaint, checked);
}

void MainWindow::on_wind_moderate_clicked(bool checked)
{
    m_tripController->booleanEvent(eWindModerate, checked);
}

void MainWindow::on_wind_brisk_clicked(bool checked)
{
    m_tripController->booleanEvent(eWindBrisk, checked);
}

void MainWindow::on_wind_hard_clicked(bool checked)
{
    m_tripController->booleanEvent(eWindHard, checked);
}

void MainWindow::on_species_currentIndexChanged(int index)
{
    m_tripController->intEvent(eSpecies, index);
}

void MainWindow::on_weight_textChanged(QString weight)
{
    m_tripController->textEvent(eWeight, weight);
}

void MainWindow::on_length_textChanged(QString length)
{
    m_tripController->textEvent(eLength, length);
}

void MainWindow::on_spotdepth_textChanged(QString depth)
{
    m_tripController->textEvent(eSpotDepth, depth);
}

void MainWindow::on_undersize_clicked(bool checked)
{
    m_tripController->booleanEvent(eUnderSize, checked);
}

void MainWindow::on_new_fish_clicked()
{
    m_tripController->buttonEvent(eNewFish);
}

void MainWindow::on_del_fish_clicked()
{
    m_tripController->buttonEvent(eDeleteFish);
}

void MainWindow::on_air_temp_start_textChanged(QString temp)
{
    m_tripController->textEvent(eStartTemp, temp);
}

void MainWindow::on_air_temp_end_textChanged(QString temp)
{
    m_tripController->textEvent(eEndTemp, temp);
}

void MainWindow::on_trip_delete_clicked()
{
    m_tripController->buttonEvent(eDeleteTrip);
}

void MainWindow::on_trip_new_clicked()
{
    m_tripController->buttonEvent(eNewTrip);
}

void MainWindow::on_method_currentIndexChanged(int index)
{
    m_tripController->intEvent(eMethod, index);
}

void MainWindow::on_trip_list_itemActivated(QListWidgetItem* item)
{
    m_tripController->intEvent(eTrip, item->type());
}

void MainWindow::on_trip_list_itemClicked(QListWidgetItem* item)
{
    m_tripController->intEvent(eTrip, item->type());
}

void MainWindow::on_fish_list_itemSelectionChanged()
{
    QList<QTableWidgetItem *> selected = ui->fish_list->selectedItems();
    if(selected.size() > 1)
        m_tripController->intEvent(eFishList, selected.at(0)->type());
}

void MainWindow::on_startDial_valueChanged(int value)
{
    m_tripController->intEvent(eStartTime, value);
}

void MainWindow::on_endDial_valueChanged(int value)
{
    m_tripController->intEvent(eEndTime, value);
}
