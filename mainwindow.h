#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QComboBox>
#include "ui_mainwindow.h"
#include "trollingmodel.h"
#include "tripcontroller.h"
#include "lurecontroller.h"
#include "placecontroller.h"
#include "lureitem.h"
#include "waypointitem.h"
#include "waypointlist.h"

namespace Ui {
    class MainWindow;
}
class UIEventHandler;
class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class UIEventHandler;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:    
    Ui::MainWindow *ui;
    TripController* m_tripController;
    LureController* m_lureController;
    PlaceController* m_placeController;
    UIEventHandler* m_eventHandler;
    LureItem* m_lureBox;
    WayPointItem* m_POIBox;
    WayPointList* m_wptList;

private slots:
    void on_user_props_cellChanged(int row, int column);
    void on_time_dial_minutes_valueChanged(int value);
    void on_time_dial_hour_valueChanged(int value);
    void on_new_weather_clicked();
    void on_new_fishweather_clicked();
    void on_place_currentIndexChanged(int index);
    void on_place_list_itemClicked(QListWidgetItem* item);
    void on_place_list_itemSelectionChanged();
    void on_place_list_itemActivated(QListWidgetItem* item);
    void on_place_delete_clicked();
    void on_place_new_clicked();
    void on_place_misc_textChanged();
    void on_place_invisible_clicked(bool checked);
    void on_place_city_textEdited(QString );
    void on_place_name_textEdited(QString );
    void on_method_textChanged(QString );
    void on_getter_textChanged(QString );
    void on_pressure_change_valueChanged(int value);
    void on_wind_direction_valueChanged(int value);
    void on_catchrelease_clicked(bool checked);
    void on_group_clicked(bool checked);
    void on_pressure_high_clicked(bool checked);
    void on_pressure_mildhigh_clicked(bool checked);
    void on_pressure_normal_clicked(bool checked);
    void on_pressure_mildlow_clicked(bool checked);
    void on_pressure_low_clicked(bool checked);
    void on_species_textChanged(QString );
    void on_timeEdit_timeChanged(QTime date);
    void on_release_width_textEdited(QString );
    void on_line_weight_textEdited(QString );
    void on_trolling_speed_textEdited(QString );
    void on_totaldepth_textEdited(QString );
    void on_lure_list_itemClicked(QListWidgetItem* item);
    void on_lure_list_itemSelectionChanged();
    void on_lure_list_itemActivated(QListWidgetItem* item);
    void on_lure_favorite_clicked(bool checked);
    void on_lure_color_textEdited(QString );
    void on_lure_size_textEdited(QString );
    void on_lure_model_textEdited(QString );
    void on_lure_manufacturer_textEdited(QString );
    void on_lure_delete_clicked();
    void on_lure_new_clicked();
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
    void on_wind_hard_clicked(bool checked);
    void on_wind_brisk_clicked(bool checked);
    void on_wind_moderate_clicked(bool checked);
    void on_wind_faint_clicked(bool checked);
    void on_wind_calm_clicked(bool checked);
    void on_weather_fog_clicked(bool checked);
    void on_weather_rain_clicked(bool checked);
    void on_weather_overcast_clicked(bool checked);
    void on_weather_halfclear_clicked(bool checked);
    void on_weather_clear_clicked(bool checked);
    void on_misc_textChanged();
    void on_water_temp_textEdited(QString );
    void on_trip_save_clicked();
    void on_dateEdit_dateChanged(QDate date);
};

#endif // MAINWINDOW_H
