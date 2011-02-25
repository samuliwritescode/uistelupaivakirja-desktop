#ifndef FISH_H
#define FISH_H
#include <QTime>
#include "lure.h"
class Trip;

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
const QString FISH_RAIN = "fish_rain";
const QString FISH_IS_GROUP = "fish_group";
const QString FISH_GROUP_AMOUNT = "fish_group_amount";
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
const QString FISH_MEDIAFILES = "fish_mediafiles";
const QString FISH_LURE = "lure";

class Fish: public QObject
{
    Q_OBJECT

    friend class Trip;
public:

    enum EWindDirection{eSouth=1, eSouthWest, eWest, eNorthWest, eNorth, eNorthEast, eEast, eSouthEast, eNoWindDirection};
    enum EPressureChange{eFastDecline=1, eModerateDecline, eSlowDecline, eNoChange, eSlowRaise, eModerateRaise, eFastRaise};
    enum EType {eNaN, eFish, eWeather, eFishAndWeather};

    Fish(Trip*);

    void setLure(Lure*);
    Lure* getLure();

    EType getType();

    QString getHumanReadableWeather();
    QString getHumanReadableWind();
    QString getHumanReadablePressure();
    QString getHumanReadableWindDirection();
    QString getHumanReadablePressureChange();
    QString getHumanReadableRain();

    void setWeight(const QString&);
    void setLength(const QString&);
    void setSpotDepth(const QString&);
    void setTotalDepth(const QString&);
    void setTrollingSpeed(const QString&);
    void setLineWeight(const QString&);
    void setReleaseWidth(const QString&);
    void setSpecies(const QString&);
    void setMethod(const QString&);
    void setGetter(const QString&);
    void setMiscText(const QString&);
    void setWaterTemp(const QString&);
    void setAirTemp(const QString&);
    void setCoordinates(const QString&, const QString&);
    void setWindCondition(int);
    void setWeatherCondition(int);
    void setPressureCondition(int);
    void setRainCondition(int);
    void setWindDirection(EWindDirection);
    void setPressureChange(EPressureChange);
    void setGroup(bool);
    void setGroupAmount(int);
    void setCR(bool);
    void setUnderSize(bool);
    void setTime(const QTime&);

    QString getWeight();
    QString getLength();
    QString getSpotDepth();
    QString getTotalDepth();
    QString getTrollingSpeed();
    QString getTrollingSpeedEstimate();
    QString getLineWeight();
    QString getReleaseWidth();
    QString getSpecies();
    QString getMethod();
    QString getGetter();
    QString getMiscText();
    QString getWaterTemp();
    QString getAirTemp();
    QString getCoordinatesLat();
    QString getCoordinatesLon();
    int getGroupAmount();
    int getWindCondition();
    int getWeatherCondition();
    int getPressureCondition();
    int getRainCondition();
    EWindDirection getWindDirection();
    EPressureChange getPressureChange();
    QTime getTime() const;
    bool isGroup();
    bool isUnderSize();
    bool isCR();

    void setUserField(const QString&, const QString&);
    QMap<QString, QString> getUserFields();
    QVariant getProperty(const QString&);

    void addMediaFile(const QString&);
    QStringList getMediaFiles();
    void removeMediaFile(const QString&);

signals:
    void FishModified();

private:
    QList<QString> getPropertyNames();
    void setDouble(const QString&, const QString&);
    void setProperty(const QString&, QVariant);
    void setType(EType type);
    Trip* m_parent;
    QHash<QString, QVariant> m_properties;
};

#endif // FISH_H
