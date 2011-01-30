#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lureitem.h"
#include "singletons.h"
#include "fish.h"
#include "uieventhandler.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    m_tripController = Singletons::tripController();
    m_lureController = Singletons::lureController();
    m_placeController = Singletons::placeController();

    ui->setupUi(this);
    ui->water_temp->setValidator(new QDoubleValidator(this));
    ui->air_temp->setValidator(new QDoubleValidator(this));
    ui->weight->setValidator(new QDoubleValidator(this));
    ui->length->setValidator(new QDoubleValidator(this));
    ui->spotdepth->setValidator(new QDoubleValidator(this));
    ui->totaldepth->setValidator(new QDoubleValidator(this));
    ui->trolling_speed->setValidator(new QDoubleValidator(this));
    ui->line_weight->setValidator(new QDoubleValidator(this));
    ui->release_width->setValidator(new QDoubleValidator(this));

    m_lureBox = new LureItem();
    m_lureBox->setAcceptDrops(true);
    ui->horizontalLayout_11->insertWidget(0, m_lureBox);

    m_POIBox = new WayPointItem();
    m_POIBox->setAcceptDrops(true);
    ui->horizontalLayout_18->insertWidget(0, m_POIBox);

    m_wptList = new WayPointList();
    m_wptList->setAcceptDrops(true);
    ui->poiDockLayout->insertWidget(0, m_wptList);

    m_eventHandler = new UIEventHandler(this);
}

MainWindow::~MainWindow()
{
    delete m_eventHandler;
    delete ui;
    delete m_lureBox;
    delete m_POIBox;
    delete m_wptList;
}


void MainWindow::on_dateEdit_dateChanged(QDate date)
{    
    m_tripController->dateEvent(eTripDate, date);
}

void MainWindow::on_trip_save_clicked()
{
    m_tripController->buttonEvent(eSaveTrip);
}

void MainWindow::on_water_temp_textEdited(QString temp)
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

void MainWindow::on_weight_textEdited(QString weight)
{
    m_tripController->textEvent(eWeight, weight);
}

void MainWindow::on_length_textEdited(QString length)
{
    m_tripController->textEvent(eLength, length);
}

void MainWindow::on_spotdepth_textEdited(QString depth)
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

void MainWindow::on_air_temp_textEdited(QString temp)
{
    m_tripController->textEvent(eAirTemp, temp);
}

void MainWindow::on_trip_delete_clicked()
{
    m_tripController->buttonEvent(eDeleteTrip);
}

void MainWindow::on_trip_new_clicked()
{
    m_tripController->buttonEvent(eNewTrip);
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

void MainWindow::on_lure_new_clicked()
{
    m_lureController->buttonEvent(eLureNew);
}

void MainWindow::on_lure_delete_clicked()
{
    m_lureController->buttonEvent(eLureDelete);
}

void MainWindow::on_lure_manufacturer_textEdited(QString text)
{
    m_lureController->textEvent(eLureMaker, text);
}

void MainWindow::on_lure_model_textEdited(QString text)
{
    m_lureController->textEvent(eLureModel, text);
}

void MainWindow::on_lure_size_textEdited(QString text)
{
    m_lureController->textEvent(eLureSize, text);
}

void MainWindow::on_lure_color_textEdited(QString text)
{
    m_lureController->textEvent(eLureColor, text);
}

void MainWindow::on_lure_favorite_clicked(bool checked)
{
    m_lureController->booleanEvent(eLureFavorite, checked);
}

void MainWindow::on_lure_list_itemActivated(QListWidgetItem* item)
{
    m_lureController->intEvent(eLureList, item->type());
}

void MainWindow::on_lure_list_itemSelectionChanged()
{
    QList<QListWidgetItem*> selected = ui->lure_list->selectedItems();
    if(selected.size() > 1)
        m_lureController->intEvent(eLureList, selected.at(0)->type());
}

void MainWindow::on_lure_list_itemClicked(QListWidgetItem* item)
{
    m_lureController->intEvent(eLureList, item->type());
}

void MainWindow::on_totaldepth_textEdited(QString value)
{
    m_tripController->textEvent(eTotalDepth, value);
}

void MainWindow::on_trolling_speed_textEdited(QString value)
{
    m_tripController->textEvent(eTrollingSpeed, value);
}

void MainWindow::on_line_weight_textEdited(QString value)
{
    m_tripController->textEvent(eLineWeight, value);
}

void MainWindow::on_release_width_textEdited(QString value)
{
    m_tripController->textEvent(eReleaseWidth, value);
}

void MainWindow::on_timeEdit_timeChanged(QTime date)
{
    m_tripController->timeEvent(eTime, date);
}

void MainWindow::on_species_textChanged(QString value)
{
    m_tripController->textEvent(eSpecies, value);
}

void MainWindow::on_pressure_low_clicked(bool checked)
{
    m_tripController->booleanEvent(ePressureLow, checked);
}

void MainWindow::on_pressure_mildlow_clicked(bool checked)
{
   m_tripController->booleanEvent(ePressureMildLow, checked);
}

void MainWindow::on_pressure_normal_clicked(bool checked)
{
    m_tripController->booleanEvent(ePressureNormal, checked);
}

void MainWindow::on_pressure_mildhigh_clicked(bool checked)
{
    m_tripController->booleanEvent(ePressureMildHigh, checked);
}

void MainWindow::on_pressure_high_clicked(bool checked)
{
    m_tripController->booleanEvent(ePressureHigh, checked);
}

void MainWindow::on_group_clicked(bool checked)
{
    m_tripController->booleanEvent(eGroup, checked);
}

void MainWindow::on_catchrelease_clicked(bool checked)
{
    m_tripController->booleanEvent(eCatchNRelease, checked);
}

void MainWindow::on_wind_direction_valueChanged(int value)
{
    m_tripController->intEvent(eWindDirection, value);
}

void MainWindow::on_pressure_change_valueChanged(int value)
{
    m_tripController->intEvent(ePressureChange, value);
}

void MainWindow::on_getter_textChanged(QString value)
{
    m_tripController->textEvent(eGetter, value);
}

void MainWindow::on_method_textChanged(QString value)
{
    m_tripController->textEvent(eMethod, value);
}

void MainWindow::on_place_name_textEdited(QString value)
{
    m_placeController->textEvent(ePlaceName, value);
}

void MainWindow::on_place_city_textEdited(QString value)
{
    m_placeController->textEvent(ePlaceCity, value);
}

void MainWindow::on_place_invisible_clicked(bool checked)
{
    m_placeController->booleanEvent(ePlaceInvisible, checked);
}

void MainWindow::on_place_misc_textChanged()
{
    m_placeController->textEvent(ePlaceMiscText, ui->place_misc->toPlainText());
}

void MainWindow::on_place_new_clicked()
{
    m_placeController->buttonEvent(ePlaceNew);
}

void MainWindow::on_place_delete_clicked()
{
    m_placeController->buttonEvent(ePlaceDelete);
}

void MainWindow::on_place_list_itemActivated(QListWidgetItem* item)
{
    m_placeController->intEvent(ePlaceList, item->type());
}

void MainWindow::on_place_list_itemSelectionChanged()
{
    QList<QListWidgetItem*> selected = ui->lure_list->selectedItems();
    if(selected.size() > 1)
        m_placeController->intEvent(ePlaceList, selected.at(0)->type());
}

void MainWindow::on_place_list_itemClicked(QListWidgetItem* item)
{
    m_placeController->intEvent(ePlaceList, item->type());
}

void MainWindow::on_place_currentIndexChanged(int index)
{
    m_tripController->intEvent(ePlaceName, ui->place->itemData(index).toInt());
}
