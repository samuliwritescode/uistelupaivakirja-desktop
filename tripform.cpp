#include "tripform.h"
#include "ui_tripform.h"

TripForm::TripForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TripForm)
{
    ui->setupUi(this);

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
    observerEvent(Controller::ePlaceListUpdated);
    observerEvent(Controller::eLureListUpdated);

    connect(m_tripController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));

    connect(Singletons::lureController(), SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    connect(Singletons::placeController(), SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
}

TripForm::~TripForm()
{
    disconnect(m_tripController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    delete ui;
    delete m_lureBox;
    delete m_POIBox;
    delete m_wptList;
}


void TripForm::observerEvent(int type)
{
    if(type == Controller::ePlaceListUpdated)
    {
         ui->place->clear();
         QList<QPair<QString, int> > places = Singletons::placeController()->getPlaceList();
        for(int loop=0; loop < places.size(); loop++)
        {
            QPair<QString, int> pair = places.at(loop);
            ui->place->addItem(pair.first, pair.second);
        }
    } else if(type == Controller::eLureListUpdated)
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
    }else if(type == Controller::eTripUpdated)
    {
        int type = m_tripController->getIntValue(eFishType);
        ui->dateEdit->setDate(m_tripController->getDateValue(eTripDate));

        ui->startDial->setValue((m_tripController->getIntValue(eStartTime)+12)%24);
        ui->timeStartLbl->setText(tr("aloitus klo ")+QString::number(m_tripController->getIntValue(eStartTime)));

        ui->endDial->setValue((m_tripController->getIntValue(eEndTime)+12)%24);
        ui->timeEndLbl->setText(tr("lopetus klo ")+QString::number(m_tripController->getIntValue(eEndTime)));

        int selectedPlace = m_tripController->getIntValue(ePlaceName);
        for(int loop=0; loop <  ui->place->count(); loop++)
        {
            if( ui->place->itemData(loop).toInt() == selectedPlace)
            {
                 ui->place->setCurrentIndex(loop);
            }
        }


        if(type == Fish::eFish)
        {
            ui->groupBoxWeather->hide();
            ui->groupBoxFish->show();
            ui->groupBoxOther->show();
            updateFish();
        }
        else if(type == Fish::eWeather)
        {
            ui->groupBoxFish->hide();
            ui->groupBoxWeather->show();
            ui->groupBoxOther->show();
            updateWeather();
        }
        else if(type == Fish::eFishAndWeather)
        {
            ui->groupBoxFish->show();
            ui->groupBoxWeather->show();
            ui->groupBoxOther->show();
            updateFish();
            updateWeather();
        }
        else
        {
            ui->groupBoxFish->hide();
            ui->groupBoxWeather->hide();
            ui->groupBoxOther->hide();
            return;
        }

        if( ui->misc->toPlainText() != m_tripController->getTextValue(eMiscText))
             ui->misc->setText(m_tripController->getTextValue(eMiscText));

        QList<QString> userValues = m_tripController->getUserFields();
        ui->user_props->clear();
        for(int loop=0; loop < userValues.count(); loop++)
        {
            QString uservalue = userValues.at(loop);

            ui->user_props->setItem(loop, 0, new QTableWidgetItem("arvo"));
            ui->user_props->setItem(loop, 1, new QTableWidgetItem(uservalue));
        }

    }
    else if(type == Controller::eTripListUpdated)
    {
         ui->trip_list->clear();
        QMap<QString, int> trips = m_tripController->getTripList();
        for(QMap<QString, int>::iterator iter = trips.begin(); iter != trips.end(); iter++)
        {
            QListWidgetItem* item = new QListWidgetItem(iter.key(),  ui->trip_list, iter.value());
             ui->trip_list->insertItem(0, item);
        }
    }
    else if (type == Controller::eFishListUpdated || type == Controller::eFishPropertyUpdated)
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
                tr("syvyys") <<
                tr("ved.syv") <<
                tr("vetonop") <<
                tr("vap.pit");

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
             case Fish::eWeather: bgcolor = QColor::fromRgb(240,240,255); break;
             case Fish::eFishAndWeather: bgcolor = QColor::fromRgb(240,240,240); break;
             default: bgcolor = QColor::fromRgb(255,255,255); break;
             }

             QTableWidgetItem* item1 = new QTableWidgetItem(format(props[FISH_TIME]), loop);
             QTableWidgetItem* item2 = new QTableWidgetItem(format(props[FISH_SPECIES]), loop);
             QTableWidgetItem* item3 = new QTableWidgetItem(props[FISH_METHOD], loop);
             QTableWidgetItem* item4 = new QTableWidgetItem(props[FISH_GETTER], loop);
             QTableWidgetItem* item5 = new QTableWidgetItem(props["lure"], loop);
             QTableWidgetItem* item6 = new QTableWidgetItem(props[FISH_WEIGHT], loop);
             QTableWidgetItem* item7 = new QTableWidgetItem(props[FISH_LENGTH], loop);
             QTableWidgetItem* item8 = new QTableWidgetItem(props[FISH_SPOT_DEPTH], loop);
             QTableWidgetItem* item9 = new QTableWidgetItem(props[FISH_TOTAL_DEPTH], loop);
             QTableWidgetItem* item10 = new QTableWidgetItem(props[FISH_TROLLING_SPEED], loop);
             QTableWidgetItem* item11 = new QTableWidgetItem(props[FISH_RELEASE_WIDTH], loop);

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
        }

        if(selectedFish >= 0)
            ui->fish_list->selectRow(selectedFish);

        ui->fish_list->setSortingEnabled(true);
        ui->fish_list->blockSignals(false);
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

void TripForm::updateFish()
{
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
    m_lureBox->setText(m_tripController->getTextValue(eLureName));
    m_POIBox->setText(m_tripController->getTextValue(eWayPointSet));

    QTime time = m_tripController->getTimeValue(eTime);
    ui->timeEdit->setTime(time);
    ui->time_dial_hour->setValue((time.hour()+12)%24);
    ui->time_dial_minutes->setValue((time.minute()+30)%60);

    setCombo(eSpecies,  ui->species);
    setCombo(eGetter,  ui->getter);
    setCombo(eMethod,  ui->method);
}

void TripForm::updateWeather()
{
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

    ui->wind_direction->setValue(m_tripController->getIntValue(eWindDirection));
    ui->pressure_change->setValue(m_tripController->getIntValue(ePressureChange));
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

void TripForm::on_weather_clear_clicked(bool checked)
{
    m_tripController->booleanEvent(eWeatherClear, checked);
}

void TripForm::on_weather_halfclear_clicked(bool checked)
{
    m_tripController->booleanEvent(eWeatherHalfClear, checked);
}

void TripForm::on_weather_overcast_clicked(bool checked)
{
    m_tripController->booleanEvent(eWeatherOvercast, checked);
}

void TripForm::on_weather_rain_clicked(bool checked)
{
    m_tripController->booleanEvent(eWeatherRain, checked);
}

void TripForm::on_weather_fog_clicked(bool checked)
{
    m_tripController->booleanEvent(eWeatherFog, checked);
}

void TripForm::on_wind_calm_clicked(bool checked)
{
    m_tripController->booleanEvent(eWindCalm, checked);
}

void TripForm::on_wind_faint_clicked(bool checked)
{
    m_tripController->booleanEvent(eWindFaint, checked);
}

void TripForm::on_wind_moderate_clicked(bool checked)
{
    m_tripController->booleanEvent(eWindModerate, checked);
}

void TripForm::on_wind_brisk_clicked(bool checked)
{
    m_tripController->booleanEvent(eWindBrisk, checked);
}

void TripForm::on_wind_hard_clicked(bool checked)
{
    m_tripController->booleanEvent(eWindHard, checked);
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

void TripForm::on_pressure_low_clicked(bool checked)
{
    m_tripController->booleanEvent(ePressureLow, checked);
}

void TripForm::on_pressure_mildlow_clicked(bool checked)
{
   m_tripController->booleanEvent(ePressureMildLow, checked);
}

void TripForm::on_pressure_normal_clicked(bool checked)
{
    m_tripController->booleanEvent(ePressureNormal, checked);
}

void TripForm::on_pressure_mildhigh_clicked(bool checked)
{
    m_tripController->booleanEvent(ePressureMildHigh, checked);
}

void TripForm::on_pressure_high_clicked(bool checked)
{
    m_tripController->booleanEvent(ePressureHigh, checked);
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

void TripForm::on_user_props_cellChanged(int row, int column)
{
    QString name;
    if(ui->user_props->item(row, 0))
        name = ui->user_props->item(row, 0)->text();

    QString value;
    if(ui->user_props->item(row, 1))
        value = ui->user_props->item(row, 1)->text();

    qDebug() << "nimi"<<name<<"arvo"<<value;
    if(!name.isEmpty())
    {
        m_tripController->textEvent(eUserField, QString::number(row)+"\n"+value);
    }

}

void TripForm::on_place_currentIndexChanged(int index)
{
    m_tripController->intEvent(ePlaceName, ui->place->itemData(index).toInt());
}
