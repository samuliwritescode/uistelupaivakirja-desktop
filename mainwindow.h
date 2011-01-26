#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include "trollingmodel.h"
#include "tripcontroller.h"
#include "lurecontroller.h"
#include "placecontroller.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TripController* m_tripController;
    LureController* m_lureController;

private slots:
    void on_lure_list_itemActivated(QListWidgetItem* item);
    void on_lure_favorite_clicked(bool checked);
    void on_lure_color_textChanged(QString );
    void on_lure_size_textChanged(QString );
    void on_lure_model_textChanged(QString );
    void on_lure_manufacturer_textChanged(QString );
    void on_lure_delete_clicked();
    void on_lure_new_clicked();
    void on_endDial_valueChanged(int value);
    void on_startDial_valueChanged(int value);
    void on_fish_list_itemSelectionChanged();
    void on_trip_list_itemClicked(QListWidgetItem* item);
    void on_trip_list_itemActivated(QListWidgetItem* item);
    void on_method_currentIndexChanged(int index);
    void observerEvent(int type);
    void observerEventLure(int type);

private slots:
    void on_trip_new_clicked();
    void on_trip_delete_clicked();
    void on_air_temp_textChanged(QString );
    void on_del_fish_clicked();
    void on_new_fish_clicked();
    void on_undersize_clicked(bool checked);
    void on_spotdepth_textChanged(QString );
    void on_length_textChanged(QString );
    void on_weight_textChanged(QString );
    void on_species_currentIndexChanged(int index);
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
    void on_water_temp_textChanged(QString );
    void on_trip_save_clicked();
    void on_place_currentIndexChanged(QString );

    void on_dateEdit_dateChanged(QDate date);
};

#endif // MAINWINDOW_H
