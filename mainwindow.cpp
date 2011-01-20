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

    ui->dateEdit->setDate(QDate::currentDate());
    ui->place->insertItems(0, Singletons::placeController()->getPlaces());



    ui->species->insertItem(0, "Hauki");
    ui->species->insertItem(0, "Ahven");
    ui->species->insertItem(0, "Kuha");


    for(int loop=0; loop < 10; loop++)
    {
        LureItem* draggable = new LureItem("HK varma");
        LureItem* poi = new LureItem("12:22 hauki 13.3-1");

        QComboBox* kalat = new QComboBox();
        kalat->insertItem(0, "hauki");
        kalat->insertItem(0, "ahven");
        draggable->setAcceptDrops(true);
      /*  ui->catchGrid->addWidget(kalat, loop, 0);
        ui->catchGrid->addWidget(new QLineEdit("6kg"), loop, 1);
        ui->catchGrid->addWidget(new QLineEdit("85cm"), loop, 2);
        ui->catchGrid->addWidget(new QLineEdit("22C"), loop, 3);
        ui->catchGrid->addWidget(new QLineEdit("5m"), loop, 4);
        ui->catchGrid->addWidget(draggable, loop, 5);
        ui->catchGrid->addWidget(poi, loop, 6);*/
    }

    //ui->lureList->setIconSize(QSize(100,50));
    for(int loop=0; loop < 100; loop++)
    {
        QListWidgetItem* item = new QListWidgetItem("HK varma taimen 9 cm venäjän lippu"+QString::number(loop));
        item->setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        item->setIcon(QIcon(":/msnlogo.jpg"));
        ui->lureList->addItem(item);
    }

    connect(m_tripController, SIGNAL(observerNotification()), this, SLOT(observerEvent()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::observerEvent()
{
    ui->dateEdit->setDate(m_tripController->getDateValue(eTripDate));
    ui->time4_9->setChecked(m_tripController->getBooleanValue(eTime49));
    qDebug() << "observer event";
}

void MainWindow::on_dateEdit_dateChanged(QDate date)
{
    qDebug() << "date was changed";
    m_tripController->dateEvent(eTripDate, ui->dateEdit->date());
}

void MainWindow::on_time4_9_clicked(bool checked)
{
    m_tripController->booleanEvent(eTime49, checked);
}

void MainWindow::on_time9_11_clicked(bool checked)
{
    m_tripController->booleanEvent(eTime911, checked);
}

void MainWindow::on_time11_14_clicked(bool checked)
{
    m_tripController->booleanEvent(eTime1114, checked);
}

void MainWindow::on_time14_18_clicked(bool checked)
{
    m_tripController->booleanEvent(eTime1418, checked);
}

void MainWindow::on_time18_23_clicked(bool checked)
{
    m_tripController->booleanEvent(eTime1823, checked);
}

void MainWindow::on_time23_4_clicked(bool checked)
{
    m_tripController->booleanEvent(eTime2304, checked);
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
