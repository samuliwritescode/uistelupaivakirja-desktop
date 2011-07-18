#include <QSettings>
#include <QFileDialog>
#include "tripform.h"
#include "ui_tripform.h"

TripForm::TripForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TripForm)
{
    ui->setupUi(this);
    ui->tabWidgetTrip->hide();

    m_tripController = Singletons::tripController();

    ui->water_temp->setValidator(new QDoubleValidator(this));
    ui->air_temp->setValidator(new QDoubleValidator(this));
    ui->weight->setValidator(new QDoubleValidator(this));
    ui->length->setValidator(new QDoubleValidator(this));
    ui->spotdepth->setValidator(new QDoubleValidator(this));
    ui->totaldepth->setValidator(new QDoubleValidator(this));
    ui->trolling_speed->setValidator(new QDoubleValidator(this));
    ui->line_weight->setValidator(new QDoubleValidator(this));
    ui->release_width->setValidator(new QDoubleValidator(this));
    ui->group_number->setValidator(new QIntValidator(this));

    m_lureBox = new LureItem();
    m_lureBox->setAcceptDrops(true);
    ui->horizontalLayout_11->insertWidget(0, m_lureBox);

    m_POIBox = new WayPointItem();
    m_POIBox->setAcceptDrops(true);
    ui->horizontalLayout_18->insertWidget(0, m_POIBox);

    m_route = new RouteItem();
    m_route->setAcceptDrops(true);
    ui->verticalLayout_5->insertWidget(1, m_route);

    m_wptList = new WayPointList();
    m_wptList->setAcceptDrops(true);
    ui->poiDockLayout->insertWidget(0, m_wptList);

    m_mediaList = new MediaList(eMediaFileAdd, eMediaFileRemove);
    m_mediaList->setAcceptDrops(true);
    ui->horizontalLayout_10->insertWidget(0, m_mediaList);
    ui->fish_list->sortByColumn(0, Qt::AscendingOrder);

    m_mediaListTrip = new MediaList(eMediaFileAddTrip, eMediaFileRemoveTrip);
    m_mediaListTrip->setAcceptDrops(true);
    ui->verticalLayout_15->insertWidget(1, m_mediaListTrip);

    observerEvent(Controller::eTripUpdated);
    observerEvent(Controller::eTripListUpdated);
    observerEvent(Controller::eFishListUpdated);
    observerEvent(Controller::ePlaceListUpdated);
    observerEvent(Controller::eLureListUpdated);

    connect(m_tripController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));

    connect(Singletons::lureController(), SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    connect(Singletons::placeController(), SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    connect(Singletons::syncController(), SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
}

TripForm::~TripForm()
{
    disconnect(m_tripController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    disconnect(Singletons::lureController(), SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    disconnect(Singletons::placeController(), SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    disconnect(Singletons::syncController(), SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    delete ui;
    delete m_lureBox;
    delete m_POIBox;
    delete m_wptList;
    delete m_mediaList;
    delete m_mediaListTrip;
    delete m_route;
}


void TripForm::observerEvent(int type)
{
    switch(type)
    {
    case Controller::ePlaceListUpdated: updatePlaceList(); break;
    case Controller::eLureListUpdated: updateLureList(); break;
    case Controller::eTripUpdated: updateTrip(); break;
    case Controller::eTripListUpdated: updateTripList(); break;
    case Controller::eFishListUpdated:
    case Controller::eFishPropertyUpdated: updateFishList(); break;
    case Controller::eWayPointsUpdated: updateWaypoints(); break;
    default: break;
    }

    ui->trip_save->setDisabled(!m_tripController->getBooleanValue(eUnsavedChanges));
    ui->sync_mobile->setDisabled(m_tripController->getBooleanValue(eUnsavedChanges));
    ui->trip_delete->setDisabled(!m_tripController->getBooleanValue(eTrip));
    ui->trip_undo->setDisabled(!m_tripController->getBooleanValue(eUnsavedChanges));
    ui->groupBoxTrip->setVisible(m_tripController->getBooleanValue(eTrip));
    ui->labelCreateNewTrip->setVisible(!m_tripController->getBooleanValue(eTrip));
    //ui->poiDock->setVisible(m_tripController->getBooleanValue(eTrip));
    //ui->lureDock->setVisible(m_tripController->getBooleanValue(eTrip));
}

void TripForm::updatePlaceList()
{
    ui->place->blockSignals(true);
    ui->place->clear();
    QList<QPair<QString, int> > places = Singletons::placeController()->getPlaceListShort();
    int currentPlace = m_tripController->getIntValue(ePlaceName);
    qSort(places);
    for(int loop=0; loop < places.size(); loop++)
    {
       QPair<QString, int> pair = places.at(loop);
       ui->place->addItem(pair.first, pair.second);
       if(currentPlace == pair.second)
       {
           ui->place->setCurrentIndex(loop);
       }
    }
    if(currentPlace == -1)
    {
        ui->place->setCurrentIndex(-1);
    }

    ui->place->blockSignals(false);
}

void TripForm::updateLureList()
{
    ui->small_lure_list->clear();
    QList<QPair<QString, int> > lures = Singletons::lureController()->getLureList();
   for(int loop=0; loop < lures.size(); loop++)
   {
       QPair<QString, int> pair = lures.at(loop);

       QListWidgetItem* itemSmall = new QListWidgetItem(pair.first,  ui->small_lure_list);
       itemSmall->setData(Qt::UserRole+1,  pair.second);
       ui->small_lure_list->insertItem(0, itemSmall);
   }
}

void TripForm::updateTrip()
{
    int type = m_tripController->getIntValue(eFishType);

    ui->dateEdit->blockSignals(true);
    ui->startDial->blockSignals(true);
    ui->endDial->blockSignals(true);
    ui->timeEditTripStart->blockSignals(true);
    ui->timeEditTripEnd->blockSignals(true);
    ui->trip_description->blockSignals(true);

    ui->dateEdit->setDate(m_tripController->getDateValue(eTripDate));
    ui->startDial->setValue((m_tripController->getIntValue(eStartTime)+12)%24);
    ui->timeEditTripStart->setTime(m_tripController->getTimeValue(eStartTime));
    ui->timeEditTripEnd->setTime(m_tripController->getTimeValue(eEndTime));
    ui->endDial->setValue((m_tripController->getIntValue(eEndTime)+12)%24);
    ui->trip_description->setPlainText(m_tripController->getTextValue(eTripDescription));

    ui->startDial->blockSignals(false);
    ui->endDial->blockSignals(false);
    ui->timeEditTripStart->blockSignals(false);
    ui->timeEditTripEnd->blockSignals(false);
    ui->trip_description->blockSignals(false);
    ui->dateEdit->blockSignals(false);    

    ui->trip_short_label->setText(m_tripController->getDateValue(eTripDate).toString("dd.MM.yyyy ")+
                                    m_tripController->getTextValue(ePlaceName)
                                  );

    m_mediaListTrip->setMediaFiles(m_tripController->getMediaFilesTrip());
    m_route->setText(m_tripController->getTextValue(eRoute));
    ui->clear_route->setDisabled(m_tripController->getTextValue(eRoute).isEmpty());

    int selectedPlace = m_tripController->getIntValue(ePlaceName);
    bool bPlaceInList = false;
    ui->place->blockSignals(true);
    ui->place->setCurrentIndex(-1);
    for(int loop=0; loop <  ui->place->count(); loop++)
    {
        if( ui->place->itemData(loop).toInt() == selectedPlace)
        {
             ui->place->setCurrentIndex(loop);
             bPlaceInList = true;
             break;
        }
    }
    ui->place->blockSignals(false);

    if(!bPlaceInList && selectedPlace > 0)
    {
        ui->place->setEditable(true);
        ui->place->setEditText(tr("Kalapaikka ei-aktiivinen"));
        ui->place->setStyleSheet("background: rgb(255,0,0);");
    }
    else
    {
        ui->place->setEditable(false);
        ui->place->setStyleSheet("");
    }

    if(type == Fish::eFish)
    {
        ui->groupBoxWeather->hide();
        ui->groupBoxFish->show();
        ui->groupBoxOther->show();
        ui->groupBoxEvent->show();
        updateFish();
    }
    else if(type == Fish::eWeather)
    {
        ui->groupBoxFish->hide();
        ui->groupBoxWeather->show();
        ui->groupBoxOther->show();
        ui->groupBoxEvent->show();
        updateWeather();
    }
    else if(type == Fish::eFishAndWeather)
    {
        ui->groupBoxFish->show();
        ui->groupBoxWeather->show();
        ui->groupBoxOther->show();
        ui->groupBoxEvent->show();
        updateFish();
        updateWeather();
    }
    else if(type == Fish::ePOI)
    {
        ui->groupBoxFish->hide();
        ui->groupBoxWeather->hide();
        ui->groupBoxOther->show();
        ui->groupBoxEvent->show();
    }
    else
    {
        if(m_tripController->getBooleanValue(eTrip))
        {
            ui->tabWidgetTrip->show();
            ui->webView->setHtml(m_tripController->getTripReport());
        }
        else
        {
            ui->tabWidgetTrip->hide();
        }
        ui->groupBoxFish->hide();
        ui->groupBoxWeather->hide();
        ui->groupBoxOther->hide();
        ui->groupBoxEvent->hide();
        return;
    }

    ui->timeEdit->blockSignals(true);

    m_POIBox->setText(m_tripController->getTextValue(eWayPointSet));
    ui->clear_poi->setDisabled(m_tripController->getTextValue(eWayPointSet).isEmpty());
    QTime time = m_tripController->getTimeValue(eTime);
    ui->timeEdit->setTime(time);
    ui->time_dial_hour->blockSignals(true);
    ui->time_dial_minutes->blockSignals(true);
    ui->time_dial_hour->setValue((time.hour()+12)%24);
    ui->time_dial_minutes->setValue((time.minute()+30)%60);
    ui->time_dial_hour->blockSignals(false);
    ui->time_dial_minutes->blockSignals(false);

    m_mediaList->setMediaFiles(m_tripController->getMediaFiles());

    ui->misc->blockSignals(true);
    if( ui->misc->toPlainText() != m_tripController->getTextValue(eMiscText))
    {
         ui->misc->setPlainText(m_tripController->getTextValue(eMiscText));
    }

    ui->misc->blockSignals(false);
    ui->timeEdit->blockSignals(false);

    QSettings settings;
    if(settings.value("use_customfields").toBool())
    {
        QMap<QString, QString> userValues = m_tripController->getUserFields();
        ui->user_props->blockSignals(true);
        ui->user_props->setVisible(true);
        ui->user_props->setSortingEnabled(false);
        ui->user_props->clear();
        QStringList userHeaders;
        userHeaders << tr("Parametri") <<
                        tr("Arvo");

        ui->user_props->setHorizontalHeaderLabels(userHeaders);

        for(int loop=0; loop < 5; loop++)
        {
            QString key = "custom_field"+QString::number(loop);
            QString keyname = settings.value(key).toString();
            QString value;

            if(userValues.contains(key))
                value = userValues[key];

            ui->user_props->setItem(loop, 0,
                                    new QTableWidgetItem(keyname
                                            )
                                    );

            ui->user_props->setItem(loop, 1, new QTableWidgetItem(value));
        }
        ui->user_props->setSortingEnabled(false);
        ui->user_props->blockSignals(false);
    }
    else
    {
        ui->user_props->setVisible(false);
    }
}

void TripForm::updateTripList()
{
   ui->trip_list->clear();
   QMap<QString, int> trips = m_tripController->getTripList();
   for(QMap<QString, int>::iterator iter = trips.begin(); iter != trips.end(); iter++)
   {
       QListWidgetItem* item = new QListWidgetItem(iter.key(),  ui->trip_list, iter.value());
       ui->trip_list->insertItem(0, item);
       if(iter.value() == m_tripController->getIntValue(eTrip))
       {
           ui->trip_list->setCurrentItem(item);
       }
   }
}

void TripForm::updateFishList()
{
    ui->fish_list->blockSignals(true);
    ui->fish_list->setSortingEnabled(false);
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
            //tr("nopeus") <<
            //tr("syvyys") <<
            tr("pilvet") <<
            tr("tuuli") <<
            tr("sade") <<
            tr("ilma") <<
            tr("vesi");

    int selectedFish = m_tripController->getIntValue(eFishList);
    ui->fish_list->setHorizontalHeaderLabels(headers);
    QList<QMap<QString, QString> > fishes = m_tripController->getFishList();
    for(int loop=0; loop < fishes.size(); loop++)
    {
        QMap<QString, QString> props = fishes.at(loop);
         ui->fish_list->insertRow(loop);

         QColor bgcolor;
         switch(props[FISH_TYPE].toInt())
         {
         case Fish::eFish: bgcolor = QColor::fromRgb(255,255,255); break;
         case Fish::eWeather: bgcolor = QColor::fromRgb(255,225,200); break;
         case Fish::eFishAndWeather: bgcolor = QColor::fromRgb(200,200,255); break;
         case Fish::ePOI: bgcolor = QColor::fromRgb(200,200,200); break;
         default: bgcolor = QColor::fromRgb(255,255,255); break;
         }

         QTableWidgetItem* item1 = new QTableWidgetItem(format(props[FISH_TIME]), loop);
         QTableWidgetItem* item2 = new QTableWidgetItem(format(props[FISH_SPECIES]), loop);
         QTableWidgetItem* item3 = new QTableWidgetItem(props[FISH_METHOD], loop);
         QTableWidgetItem* item4 = new QTableWidgetItem(props[FISH_GETTER], loop);
         QTableWidgetItem* item5 = new QTableWidgetItem(props["lure"], loop);
         QTableWidgetItem* item6 = new QTableWidgetItem(props[FISH_WEIGHT], loop);
         QTableWidgetItem* item7 = new QTableWidgetItem(props[FISH_LENGTH], loop);
         //QTableWidgetItem* item8 = new QTableWidgetItem(props[FISH_TROLLING_SPEED], loop);
         //QTableWidgetItem* item9 = new QTableWidgetItem(props[FISH_SPOT_DEPTH], loop);
         QTableWidgetItem* item8 = new QTableWidgetItem(props[FISH_WEATHER], loop);
         QTableWidgetItem* item9 = new QTableWidgetItem(props[FISH_WIND], loop);
         QTableWidgetItem* item10 = new QTableWidgetItem(props[FISH_RAIN], loop);
         QTableWidgetItem* item11 = new QTableWidgetItem(props[FISH_AIR_TEMP], loop);
         QTableWidgetItem* item12 = new QTableWidgetItem(props[FISH_WATER_TEMP], loop);

         item1->setBackgroundColor(bgcolor);
         item2->setBackgroundColor(bgcolor);
         item3->setBackgroundColor(bgcolor);
         item4->setBackgroundColor(bgcolor);
         item5->setBackgroundColor(bgcolor);
         item6->setBackgroundColor(bgcolor);
         item7->setBackgroundColor(bgcolor);
         item8->setBackgroundColor(bgcolor);
         item9->setBackgroundColor(bgcolor);
         item10->setBackgroundColor(bgcolor);
         item11->setBackgroundColor(bgcolor);
         item12->setBackgroundColor(bgcolor);
         //item13->setBackgroundColor(bgcolor);
         //item14->setBackgroundColor(bgcolor);

         ui->fish_list->setItem(loop, 0, item1);
         ui->fish_list->setItem(loop, 1, item2);
         ui->fish_list->setItem(loop, 2, item3);
         ui->fish_list->setItem(loop, 3, item4);
         ui->fish_list->setItem(loop, 4, item5);
         ui->fish_list->setItem(loop, 5, item6);
         ui->fish_list->setItem(loop, 6, item7);
         ui->fish_list->setItem(loop, 7, item8);
         ui->fish_list->setItem(loop, 8, item9);
         ui->fish_list->setItem(loop, 9, item10);
         ui->fish_list->setItem(loop, 10, item11);
         ui->fish_list->setItem(loop, 11, item12);
         //ui->fish_list->setItem(loop, 12, item13);
         //ui->fish_list->setItem(loop, 13, item14);
    }

    if(selectedFish >= 0)
        ui->fish_list->selectRow(selectedFish);

    ui->fish_list->setSortingEnabled(true);
    ui->fish_list->blockSignals(false);
}

void TripForm::updateWaypoints()
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

    ui->routept_clear->setEnabled(m_tripController->getWayPointsList().count() > 0);
}

void TripForm::updateFish()
{
    ui->group_number->setDisabled(!m_tripController->getBooleanValue(eGroup));
    ui->group_label->setDisabled(!m_tripController->getBooleanValue(eGroup));
    ui->length->setDisabled(m_tripController->getBooleanValue(eGroup));
    ui->label_length->setDisabled(m_tripController->getBooleanValue(eGroup));

    ui->group_number->setText(QString::number(m_tripController->getIntValue(eGroup)));

    ui->length->setText(m_tripController->getTextValue(eLength));
    ui->weight->setText(m_tripController->getTextValue(eWeight));
    ui->spotdepth->setText(m_tripController->getTextValue(eSpotDepth));
    ui->totaldepth->setText(m_tripController->getTextValue(eTotalDepth));
    ui->trolling_speed->setText(m_tripController->getTextValue(eTrollingSpeed));
    ui->trolling_speed->setPlaceholderText(m_tripController->getTextValue(eTrollingSpeedEstimate));
    ui->line_weight->setText(m_tripController->getTextValue(eLineWeight));
    ui->release_width->setText(m_tripController->getTextValue(eReleaseWidth));

    ui->group->setChecked(m_tripController->getBooleanValue(eGroup));
    ui->undersize->setChecked(m_tripController->getBooleanValue(eUnderSize));
    ui->catchrelease->setChecked(m_tripController->getBooleanValue(eCatchNRelease));
    m_lureBox->setText(m_tripController->getTextValue(eLureName));

    setCombo(eSpecies,  ui->species);
    setCombo(eGetter,  ui->getter);
    setCombo(eMethod,  ui->method);
}

void TripForm::updateWeather()
{
    ui->cloud_slider->blockSignals(true);
    ui->rain_slider->blockSignals(true);
    ui->wind_slider->blockSignals(true);
    ui->pressure_slider->blockSignals(true);
    ui->cloud_slider->setValue(m_tripController->getIntValue(eWeather));
    ui->cloud_label->setText(m_tripController->getTextValue(eWeather));
    ui->rain_slider->setValue(m_tripController->getIntValue(eRain));
    ui->rain_label->setText(m_tripController->getTextValue(eRain));
    ui->pressure_slider->setValue(m_tripController->getIntValue(ePressure));
    ui->pressure_label->setText(m_tripController->getTextValue(ePressure));
    ui->wind_slider->setValue(m_tripController->getIntValue(eWind));
    ui->wind_label->setText(m_tripController->getTextValue(eWind));

    ui->cloud_slider->blockSignals(false);
    ui->rain_slider->blockSignals(false);
    ui->wind_slider->blockSignals(false);
    ui->pressure_slider->blockSignals(false);

    ui->wind_direction->setValue(m_tripController->getIntValue(eWindDirection));
    ui->pressure_change->setValue(m_tripController->getIntValue(ePressureChange));
    ui->wind_direction_label->setText(m_tripController->getTextValue(eWindDirection));
    ui->pressure_change_label->setText(m_tripController->getTextValue(ePressureChange));

    ui->air_temp->setText(m_tripController->getTextValue(eAirTemp));
    ui->water_temp->setText(m_tripController->getTextValue(eWaterTemp));
}

void TripForm::setCombo(EUISource source, QComboBox* target)
{
    QStringList valuelist = m_tripController->getAlternatives(source);
    QString currentValue = m_tripController->getTextValue(source);
    target->blockSignals(true);
    foreach(QString value, valuelist)
    {
        if(target->findText(value) == -1)
        {
            target->addItem(value);
        }
    }
    target->setEditText(currentValue);
    target->blockSignals(false);
}

QString TripForm::format(const QString& str)
{
    if(str == "")
        return "n/a";
    return str;
}

void TripForm::on_dateEdit_dateChanged(QDate date)
{
    m_tripController->dateEvent(eTripDate, date);
}

void TripForm::on_trip_save_clicked()
{
    m_tripController->buttonEvent(eSaveTrip);
}

void TripForm::on_water_temp_textEdited(QString temp)
{
    m_tripController->textEvent(eWaterTemp, temp);
}

void TripForm::on_misc_textChanged()
{
    m_tripController->textEvent(eMiscText, ui->misc->toPlainText());
}

void TripForm::on_weight_textEdited(QString weight)
{
    m_tripController->textEvent(eWeight, weight);
}

void TripForm::on_length_textEdited(QString length)
{
    m_tripController->textEvent(eLength, length);
}

void TripForm::on_spotdepth_textEdited(QString depth)
{
    m_tripController->textEvent(eSpotDepth, depth);
}

void TripForm::on_undersize_clicked(bool checked)
{
    m_tripController->booleanEvent(eUnderSize, checked);
}

void TripForm::on_new_fish_clicked()
{
    m_tripController->buttonEvent(eNewFish);
}

void TripForm::on_del_fish_clicked()
{
    m_tripController->buttonEvent(eDeleteFish);
}

void TripForm::on_air_temp_textEdited(QString temp)
{
    m_tripController->textEvent(eAirTemp, temp);
}

void TripForm::on_trip_delete_clicked()
{
    m_tripController->buttonEvent(eDeleteTrip);
}

void TripForm::on_trip_new_clicked()
{
    m_tripController->buttonEvent(eNewTrip);
    ui->tabWidgetTrip->setCurrentIndex(1);
}

void TripForm::on_trip_list_itemActivated(QListWidgetItem* item)
{
    m_tripController->intEvent(eTrip, item->type());
}

void TripForm::on_trip_list_itemClicked(QListWidgetItem* item)
{
    m_tripController->intEvent(eTrip, item->type());
}

void TripForm::on_fish_list_itemSelectionChanged()
{
    QList<QTableWidgetItem *> selected = ui->fish_list->selectedItems();
    if(selected.size() > 1)
        m_tripController->intEvent(eFishList, selected.at(0)->type());
}

void TripForm::on_startDial_valueChanged(int value)
{
    m_tripController->intEvent(eStartTime, (value+12)%24);
}

void TripForm::on_endDial_valueChanged(int value)
{
    m_tripController->intEvent(eEndTime, (value+12)%24);
}

void TripForm::on_totaldepth_textEdited(QString value)
{
    m_tripController->textEvent(eTotalDepth, value);
}

void TripForm::on_trolling_speed_textEdited(QString value)
{
    m_tripController->textEvent(eTrollingSpeed, value);
}

void TripForm::on_line_weight_textEdited(QString value)
{
    m_tripController->textEvent(eLineWeight, value);
}

void TripForm::on_release_width_textEdited(QString value)
{
    m_tripController->textEvent(eReleaseWidth, value);
}

void TripForm::on_timeEdit_timeChanged(QTime date)
{
    m_tripController->timeEvent(eTime, date);
}

void TripForm::on_species_textChanged(QString value)
{
    m_tripController->textEvent(eSpecies, value);
}

void TripForm::on_group_clicked(bool checked)
{
    m_tripController->booleanEvent(eGroup, checked);
}

void TripForm::on_catchrelease_clicked(bool checked)
{
    m_tripController->booleanEvent(eCatchNRelease, checked);
}

void TripForm::on_wind_direction_valueChanged(int value)
{
    m_tripController->intEvent(eWindDirection, value);
}

void TripForm::on_pressure_change_valueChanged(int value)
{
    m_tripController->intEvent(ePressureChange, value);
}

void TripForm::on_getter_textChanged(QString value)
{
    m_tripController->textEvent(eGetter, value);
}

void TripForm::on_method_textChanged(QString value)
{
    m_tripController->textEvent(eMethod, value);
}

void TripForm::on_new_fishweather_clicked()
{
    m_tripController->buttonEvent(eNewFishWeather);
}

void TripForm::on_new_weather_clicked()
{
    m_tripController->buttonEvent(eNewWeather);
}

void TripForm::on_user_props_cellChanged(int row, int column)
{
    QString name = "custom_field"+QString::number(row);
    QString value;

    if(ui->user_props->item(row, 1))
        value = ui->user_props->item(row, 1)->text();

    qDebug() << "nimi"<<name<<"arvo"<<value;
    m_tripController->textEvent(eUserField, name+"\n"+value);

}

void TripForm::on_place_currentIndexChanged(int index)
{
    m_tripController->intEvent(ePlaceName, ui->place->itemData(index).toInt());
}

void TripForm::on_trip_undo_clicked()
{
    m_tripController->buttonEvent(eTripUndo);
}

void TripForm::on_group_number_textEdited(QString value)
{
    m_tripController->intEvent(eGroup, value.toInt());
}

void TripForm::on_cloud_slider_valueChanged(int position)
{
    m_tripController->intEvent(eWeather, position);
}

void TripForm::on_wind_slider_valueChanged(int position)
{
     m_tripController->intEvent(eWind, position);
}

void TripForm::on_pressure_slider_valueChanged(int position)
{
     m_tripController->intEvent(ePressure, position);
}

void TripForm::on_rain_slider_valueChanged(int position)
{
     m_tripController->intEvent(eRain, position);
}

void TripForm::on_timeEditTripStart_timeChanged(QTime time)
{
    m_tripController->timeEvent(eStartTime, time);
}

void TripForm::on_timeEditTripEnd_timeChanged(QTime time)
{
    m_tripController->timeEvent(eEndTime, time);
}

void TripForm::on_tripreport_back_clicked()
{
    ui->webView->setHtml(m_tripController->getTripReport());
}

void TripForm::on_tripreport_showimg_toggled(bool checked)
{
    m_tripController->booleanEvent(eTripReportShowImages, checked);
    ui->webView->setHtml(m_tripController->getTripReport());
}

void TripForm::on_tripreport_showmap_toggled(bool checked)
{
    m_tripController->booleanEvent(eTripReportShowMaps, checked);
    ui->webView->setHtml(m_tripController->getTripReport());
}

void TripForm::on_trip_description_textChanged()
{
    m_tripController->textEvent(eTripDescription, ui->trip_description->toPlainText());
}

void TripForm::on_clear_poi_clicked()
{
    m_tripController->buttonEvent(eWayPointClear);
}

void TripForm::on_clear_route_clicked()
{
    m_tripController->buttonEvent(eRouteClear);
}

void TripForm::on_searchLure_textEdited(QString text)
{
    Singletons::lureController()->textEvent(eLureSearch, text);
}

void TripForm::on_onlyFavoriteLures_clicked(bool checked)
{
    Singletons::lureController()->booleanEvent(eLureSearchFavorites, checked);
}

void TripForm::on_time_dial_hour_valueChanged(int value)
{
    QTime time;
    time.setHMS((value+12)%24, ui->timeEdit->time().minute(), 0);
    m_tripController->timeEvent(eTime, time);
}

void TripForm::on_time_dial_minutes_valueChanged(int value)
{
    QTime time;
    time.setHMS(ui->timeEdit->time().hour(), (value+30)%60, 0);
    m_tripController->timeEvent(eTime, time);
}

void TripForm::on_sync_mobile_clicked()
{
    m_tripController->buttonEvent(eMobileSync);
}

void TripForm::on_add_route_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Avaa reittiloki"), "", tr("Reittilokit (*.gpx)"));
    if(QFile::exists(filename))
    {
        m_route->insertFile(QUrl(filename));
    }
}

void TripForm::on_routept_open_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Avaa reittipistetiedosto"), "", tr("Reittipisteet (*.gpx)"));
    if(QFile::exists(filename))
    {
        m_wptList->insertFile(QUrl(filename));
    }
}

void TripForm::on_routept_clear_clicked()
{
    m_tripController->textEvent(eWaypointsAdd, "");
}

void TripForm::on_trip_file_add_clicked()
{
    m_mediaListTrip->insertMediaFile();
}

void TripForm::on_fish_file_add_clicked()
{
    m_mediaList->insertMediaFile();
}

void TripForm::on_new_poi_clicked()
{
    m_tripController->buttonEvent(eNewPOI);
}
