#ifndef TRIPFORM_H
#define TRIPFORM_H

#include <QWidget>
#include <QComboBox>
#include "lureitem.h"
#include "waypointitem.h"
#include "waypointlist.h"
#include "routeitem.h"
#include "medialist.h"
#include "singletons.h"

namespace Ui {
    class TripForm;
}

class TripForm : public QWidget
{
    Q_OBJECT

public:
    explicit TripForm(QWidget *parent = 0);
    ~TripForm();

private:
    void updatePlaceList();
    void updateLureList();
    void updateTrip();
    void updateTripList();
    void updateFishList();
    void updateWaypoints();
    void updateFish();
    void updateWeather();
    QString format(const QString& str);
    void setCombo(EUISource source, QComboBox* target);
    Ui::TripForm *ui;
    LureItem* m_lureBox;
    WayPointItem* m_POIBox;
    WayPointList* m_wptList;
    RouteItem* m_route;
    MediaList* m_mediaList;
    MediaList* m_mediaListTrip;
    TripController* m_tripController;

private slots:
    void on_fish_file_add_clicked();
    void on_trip_file_add_clicked();
    void on_routept_clear_clicked();
    void on_routept_open_clicked();
    void on_add_route_clicked();
    void on_sync_mobile_clicked();
    void on_time_dial_minutes_valueChanged(int value);
    void on_time_dial_hour_valueChanged(int value);    
    void on_onlyFavoriteLures_clicked(bool checked);
    void on_searchLure_textEdited(QString );
    void on_clear_route_clicked();
    void on_clear_poi_clicked();
    void on_trip_description_textChanged();
    void on_tripreport_showmap_toggled(bool checked);
    void on_tripreport_showimg_toggled(bool checked);
    void on_tripreport_back_clicked();
    void on_timeEditTripEnd_timeChanged(QTime date);
    void on_timeEditTripStart_timeChanged(QTime date);
    void on_rain_slider_valueChanged(int position);
    void on_pressure_slider_valueChanged(int position);
    void on_wind_slider_valueChanged(int position);
    void on_cloud_slider_valueChanged(int position);
    void on_group_number_textEdited(QString );
    void on_trip_undo_clicked();
    void observerEvent(int type);

    void on_user_props_cellChanged(int row, int column);
    void on_new_weather_clicked();
    void on_new_fishweather_clicked();
    void on_place_currentIndexChanged(int index);

    void on_method_textChanged(QString );
    void on_getter_textChanged(QString );
    void on_pressure_change_valueChanged(int value);
    void on_wind_direction_valueChanged(int value);
    void on_catchrelease_clicked(bool checked);
    void on_group_clicked(bool checked);
    void on_species_textChanged(QString );
    void on_timeEdit_timeChanged(QTime date);
    void on_release_width_textEdited(QString );
    void on_line_weight_textEdited(QString );
    void on_trolling_speed_textEdited(QString );
    void on_totaldepth_textEdited(QString );
    void on_endDial_valueChanged(int value);
    void on_startDial_valueChanged(int value);
    void on_fish_list_itemSelectionChanged();
    void on_trip_list_itemClicked(QListWidgetItem* item);
    void on_trip_list_itemActivated(QListWidgetItem* item);
    void on_trip_new_clicked();
    void on_trip_delete_clicked();
    void on_air_temp_textEdited(QString );
    void on_del_fish_clicked();
    void on_new_fish_clicked();
    void on_undersize_clicked(bool checked);
    void on_spotdepth_textEdited(QString );
    void on_length_textEdited(QString );
    void on_weight_textEdited(QString );
    void on_misc_textChanged();
    void on_water_temp_textEdited(QString );
    void on_trip_save_clicked();
    void on_dateEdit_dateChanged(QDate date);
};

#endif // TRIPFORM_H
