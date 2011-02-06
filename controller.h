#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QDate>

enum EUISource {eTripDate,
                eStartTime,
                eEndTime,
                ePlaceText,
                eWaterTemp,
                eTrip,
                eMiscText,
                eWeatherClear,
                eWeatherHalfClear,
                eWeatherOvercast,
                eWeatherRain,
                eWeatherFog,
                eWindCalm,
                eWindFaint,
                eWindModerate,
                eWindBrisk,
                eWindHard,
                eSpecies,
                eWeight,
                eLength,
                eSpotDepth,
                eUnderSize,
                eAirTemp,
                eSaveTrip,
                eDeleteFish,
                eNewFish,
                eNewWeather,
                eNewFishWeather,
                eFishType,
                eDeleteTrip,
                eNewTrip,
                eMethod,
                eFishList,
                eLureMaker,
                eLureModel,
                eLureSize,
                eLureColor,
                eLureFavorite,
                eLureNew,
                eLureDelete,
                eLureList,
                eSelectLure,
                eLureName,
                eTotalDepth,
                eTrollingSpeed,
                eLineWeight,
                eReleaseWidth,
                eTime,
                ePressureLow,
                ePressureMildLow,
                ePressureNormal,
                ePressureMildHigh,
                ePressureHigh,
                eGroup,
                eCatchNRelease,
                eWindDirection,
                ePressureChange,
                eGetter,
                ePlaceName,
                ePlaceCity,
                ePlaceInvisible,
                ePlaceMiscText,
                ePlaceNew,
                ePlaceDelete,
                ePlaceList,
                eWaypointsAdd,
                eWayPointSet,
                eUnsavedChanges,
                eUserField,
                eStatistics,
                eStatisticsUnit,
                eStatisticsColumn,
                eStatisticsField};

class Controller : public QObject
{
    Q_OBJECT
public:

    virtual void buttonEvent(EUISource);
    virtual void booleanEvent(EUISource, bool);
    virtual void dateEvent(EUISource, const QDate&);
    virtual void timeEvent(EUISource, const QTime&);
    virtual void textEvent(EUISource, const QString&);
    virtual void intEvent(EUISource, int);

    virtual QDate getDateValue(EUISource);
    virtual QTime getTimeValue(EUISource);
    virtual bool getBooleanValue(EUISource);
    virtual int getIntValue(EUISource);
    virtual QString getTextValue(EUISource);

    enum ENotificationType {
        eTripUpdated,
        eTripListUpdated,
        eFishListUpdated,
        eFishPropertyUpdated,
        eLureListUpdated,
        eLureUpdated,
        ePlaceUpdated,
        ePlaceListUpdated,
        eWayPointsUpdated,
        eStatisticsUpdated
    };

signals:
    void observerNotification(int);

public slots:

protected:
    virtual void sendNotificationToObservers(ENotificationType type);
    //This is an abstract class
     explicit Controller(QObject *parent = 0);

};

#endif // CONTROLLER_H
