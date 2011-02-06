#ifndef FISH_H
#define FISH_H
#include <QTime>
#include "lure.h"

const QString FISH_TYPE = "type";
const QString FISH_WIND = "fish_wind";
const QString FISH_WEATHER = "fish_weather";
const QString FISH_LENGTH = "fish_length";
const QString FISH_WEIGHT = "fish_weight";
const QString FISH_SPOT_DEPTH = "fish_spot_depth";
const QString FISH_MISC_TEXT = "fish_misc_text";
const QString FISH_WATER_TEMP = "fish_water_temp";
const QString FISH_AIR_TEMP = "fish_air_temp";
const QString FISH_TOTAL_DEPTH = "fish_total_depth";
const QString FISH_TROLLING_SPEED = "fish_trolling_speed";
const QString FISH_LINE_WEIGHT = "fish_line_weight";
const QString FISH_RELEASE_WIDTH = "fish_release_width";
const QString FISH_SPECIES = "fish_species";
const QString FISH_PRESSURE = "fish_pressure";
const QString FISH_IS_GROUP = "fish_group";
const QString FISH_IS_UNDERSIZE = "fish_undersize";
const QString FISH_IS_CATCHRELEASED = "fish_cr";
const QString FISH_WIND_DIRECTION = "fish_wind_direction";
const QString FISH_PRESSURE_CHANGE = "fish_pressure_change";
const QString FISH_TIME = "fish_time";
const QString FISH_GETTER = "fish_getter";
const QString FISH_METHOD = "fish_method";
const QString FISH_COORDINATES_LAT = "fish_coord_lat";
const QString FISH_COORDINATES_LON = "fish_coord_lon";
const QString FISH_USERFIELD = "fish_user";

class Fish
{
    friend class Trip;
public:

    enum EWindCondition{eCalm=1, eFaint=2, eModerate=4, eBrisk=8, eHard=16};
    enum EWeatherCondition{eClear=1, eHalfClear=2, eOvercast=4, eRain=8, eFog=16};
    enum EPressureCondition{eLow=1, eMildLow=2, eNormal=4, eMildHigh=8, eHigh=16};
    enum EType {eNaN, eFish, eWeather, eFishAndWeather};

    Fish();

    void setLure(Lure*);
    Lure* getLure();

    EType getType();

    QString getHumanReadableWeather();
    QString getHumanReadableWind();
    QString getHumanReadablePressure();

    void setProperty(const QString&, double);
    void setProperty(const QString&, QString);
    void setProperty(const QString&, int);
    void setProperty(const QString&, bool);
    void setProperty(const QString&, QTime);

    void setUserField(const QString&, const QString&);
    QMap<QString, QString> getUserFields();

    QVariant getProperty(const QString&);

    double getPropertyDouble(const QString&);
    bool isProperty(const QString&, QVariant);

    QList<QString> getPropertyNames();

private:
    QStringList m_stringprops;
    QStringList m_timeprops;
    QStringList m_intprops;
    QStringList m_boolprops;
    QStringList m_doubleprops;
    void setProperty(const QString&, QVariant);
    void setType(EType type);
    Lure* m_lure;
    QHash<QString, QVariant> m_properties;
};

#endif // FISH_H
