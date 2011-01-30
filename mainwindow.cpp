#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lureitem.h"
#include "singletons.h"
#include "fish.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_tripController(NULL)
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

    observerEvent(Controller::eTripUpdated);
    observerEvent(Controller::eTripListUpdated);
    observerEvent(Controller::eFishListUpdated);
    observerEventLure(Controller::eLureListUpdated);
    observerEventPlace(Controller::ePlaceListUpdated);

    connect(m_tripController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    connect(m_lureController, SIGNAL(observerNotification(int)), this, SLOT(observerEventLure(int)));
    connect(m_placeController, SIGNAL(observerNotification(int)), this, SLOT(observerEventPlace(int)));
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

        ui->pressure_low->setChecked(m_tripController->getBooleanValue(ePressureLow));
        ui->pressure_mildlow->setChecked(m_tripController->getBooleanValue(ePressureMildLow));
        ui->pressure_normal->setChecked(m_tripController->getBooleanValue(ePressureNormal));
        ui->pressure_mildhigh->setChecked(m_tripController->getBooleanValue(ePressureMildHigh));
        ui->pressure_high->setChecked(m_tripController->getBooleanValue(ePressureHigh));

        ui->length->setText(m_tripController->getTextValue(eLength));
        ui->weight->setText(m_tripController->getTextValue(eWeight));
        ui->spotdepth->setText(m_tripController->getTextValue(eSpotDepth));
        ui->totaldepth->setText(m_tripController->getTextValue(eTotalDepth));
        ui->trolling_speed->setText(m_tripController->getTextValue(eTrollingSpeed));
        ui->line_weight->setText(m_tripController->getTextValue(eLineWeight));
        ui->release_width->setText(m_tripController->getTextValue(eReleaseWidth));
        ui->air_temp->setText(m_tripController->getTextValue(eAirTemp));
        ui->water_temp->setText(m_tripController->getTextValue(eWaterTemp));

        ui->group->setChecked(m_tripController->getBooleanValue(eGroup));
        ui->undersize->setChecked(m_tripController->getBooleanValue(eUnderSize));
        ui->catchrelease->setChecked(m_tripController->getBooleanValue(eCatchNRelease));

        ui->wind_direction->setValue(m_tripController->getIntValue(eWindDirection));
        ui->pressure_change->setValue(m_tripController->getIntValue(ePressureChange));

        ui->timeEdit->setTime(m_tripController->getTimeValue(eTime));

        if(ui->misc->toPlainText() != m_tripController->getTextValue(eMiscText))
            ui->misc->setText(m_tripController->getTextValue(eMiscText));


        m_lureBox->setText(m_tripController->getTextValue(eLureName));
        m_POIBox->setText(m_tripController->getTextValue(eWayPointSet));

        setCombo(eSpecies, ui->species);
        setCombo(eGetter, ui->getter);
        setCombo(eMethod, ui->method);

        int selectedPlace = m_tripController->getIntValue(ePlaceName);
        for(int loop=0; loop < ui->place->count(); loop++)
        {
            if(ui->place->itemData(loop).toInt() == selectedPlace)
            {
                ui->place->setCurrentIndex(loop);
            }
        }

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
        ui->fish_list->clear();
        ui->fish_list->clearContents();
        ui->fish_list->setRowCount(0);
        QStringList headers;
        headers << tr("aika") <<
                tr("laji") <<
                tr("tapa") <<
                tr("saaja") <<
                tr("viehe") <<
                tr("paino") <<
                tr("pituus") <<
                tr("syvyys") <<
                tr("ved.syv") <<
                tr("vetonop") <<
                tr("vap.pit");
        ui->fish_list->setHorizontalHeaderLabels(headers);
        QList<QMap<QString, QString> > fishes = m_tripController->getFishList();
        for(int loop=0; loop < fishes.size(); loop++)
        {
            QMap<QString, QString> props = fishes.at(loop);
            ui->fish_list->insertRow(loop);
            ui->fish_list->setItem(loop, 0, new QTableWidgetItem(props[FISH_TIME], loop));
            ui->fish_list->setItem(loop, 1, new QTableWidgetItem(props[FISH_SPECIES], loop));
            ui->fish_list->setItem(loop, 2, new QTableWidgetItem(props[FISH_METHOD], loop));
            ui->fish_list->setItem(loop, 3, new QTableWidgetItem(props[FISH_GETTER], loop));
            ui->fish_list->setItem(loop, 4, new QTableWidgetItem(props["lure"], loop));
            ui->fish_list->setItem(loop, 5, new QTableWidgetItem(props[FISH_WEIGHT], loop));
            ui->fish_list->setItem(loop, 6, new QTableWidgetItem(props[FISH_LENGTH], loop));
            ui->fish_list->setItem(loop, 7, new QTableWidgetItem(props[FISH_SPOT_DEPTH], loop));
            ui->fish_list->setItem(loop, 8, new QTableWidgetItem(props[FISH_TOTAL_DEPTH], loop));
            ui->fish_list->setItem(loop, 9, new QTableWidgetItem(props[FISH_TROLLING_SPEED], loop));
            ui->fish_list->setItem(loop, 10, new QTableWidgetItem(props[FISH_RELEASE_WIDTH], loop));
        }
    }
    else if(type == Controller::eWayPointsUpdated)
    {
        m_wptList->clear();
        QList<QPair<QString, int> > list = m_tripController->getWayPointsList();
        for(int loop=0; loop < list.count(); loop++)
        {
            QPair<QString, int> pair = list.at(loop);
            QListWidgetItem* item = new QListWidgetItem(pair.first, m_wptList);
            item->setData(Qt::UserRole+2,  pair.second);
            m_wptList->insertItem(0, item);
        }
    }
}

void MainWindow::setCombo(EUISource source, QComboBox* target)
{
    QStringList valuelist = m_tripController->getAlternatives(source);
    QString currentValue = m_tripController->getTextValue(source);
    foreach(QString value, valuelist)
    {
        if(target->findText(value) == -1)
        {
            target->addItem(value);
        }
    }
    target->setEditText(currentValue);
}

void MainWindow::observerEventLure(int type)
{
    if(type == Controller::eLureListUpdated)
    {
        ui->lure_list->clear();
        ui->small_lure_list->clear();
        QList<QPair<QString, int> > lures = m_lureController->getLureList();
        for(int loop=0; loop < lures.size(); loop++)
        {
            QPair<QString, int> pair = lures.at(loop);
            QListWidgetItem* item = new QListWidgetItem(pair.first, ui->lure_list, pair.second);
            ui->lure_list->insertItem(0, item);

            QListWidgetItem* itemSmall = new QListWidgetItem(pair.first, ui->small_lure_list);
            itemSmall->setData(Qt::UserRole+1,  pair.second);
            ui->small_lure_list->insertItem(0, itemSmall);
        }
    } else if(type == Controller::eLureUpdated)
    {
        ui->lure_manufacturer->setText(m_lureController->getTextValue(eLureMaker));
        ui->lure_model->setText(m_lureController->getTextValue(eLureModel));
        ui->lure_size->setText(m_lureController->getTextValue(eLureSize));
        ui->lure_color->setText(m_lureController->getTextValue(eLureColor));
        ui->lure_favorite->setChecked(m_lureController->getBooleanValue(eLureFavorite));
    }
}

void MainWindow::observerEventPlace(int type)
{
    if(type == Controller::ePlaceListUpdated)
    {
        ui->place_list->clear();
        ui->place->clear();
        QList<QPair<QString, int> > places = m_placeController->getPlaceList();
        for(int loop=0; loop < places.size(); loop++)
        {
            QPair<QString, int> pair = places.at(loop);
            QListWidgetItem* item = new QListWidgetItem(pair.first, ui->place_list, pair.second);
            ui->place_list->insertItem(0, item);

            ui->place->addItem(pair.first, pair.second);
        }
    } else if(type == Controller::ePlaceUpdated)
    {
        ui->place_name->setText( m_placeController->getTextValue(ePlaceName) );
        ui->place_city->setText( m_placeController->getTextValue(ePlaceCity) );
        ui->place_misc->setText( m_placeController->getTextValue(ePlaceMiscText) );
        ui->place_invisible->setChecked(m_placeController->getBooleanValue(ePlaceInvisible));
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

void MainWindow::on_air_temp_textChanged(QString temp)
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

void MainWindow::on_lure_manufacturer_textChanged(QString text)
{
    m_lureController->textEvent(eLureMaker, text);
}

void MainWindow::on_lure_model_textChanged(QString text)
{
    m_lureController->textEvent(eLureModel, text);
}

void MainWindow::on_lure_size_textChanged(QString text)
{
    m_lureController->textEvent(eLureSize, text);
}

void MainWindow::on_lure_color_textChanged(QString text)
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

void MainWindow::on_totaldepth_textChanged(QString value)
{
    m_tripController->textEvent(eTotalDepth, value);
}

void MainWindow::on_trolling_speed_textChanged(QString value)
{
    m_tripController->textEvent(eTrollingSpeed, value);
}

void MainWindow::on_line_weight_textChanged(QString value)
{
    m_tripController->textEvent(eLineWeight, value);
}

void MainWindow::on_release_width_textChanged(QString value)
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

void MainWindow::on_place_name_textChanged(QString value)
{
    m_placeController->textEvent(ePlaceName, value);
}

void MainWindow::on_place_city_textChanged(QString value)
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
