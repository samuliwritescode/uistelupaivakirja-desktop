#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QDate>
#include "messagedisplay.h"

enum EUISource {eTripDate,
                eStartTime,
                eEndTime,
                ePlaceText,
                eWaterTemp,
                eTrip,
                eMiscText,               
                eSpecies,
                eWeight,
                eLength,
                eSpotDepth,
                eUnderSize,
                eWeather,
                eRain,
                ePressure,
                eWind,
                eAirTemp,
                eSaveTrip,
                eTripUndo,
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
                ePlaceUndo,
                ePlaceSave,
                ePlaceList,
                eWaypointsAdd,
                eWayPointSet,
                eMediaFileAdd,
                eMediaFileRemove,
                eUnsavedChanges,
                eUserField,
                eStatistics,
                eStatisticsUnit,
                eStatisticsColumn,
                eStatisticsByColumn,
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

    void setMessageDisplay(MessageDisplay*);
    void showErrorMessage(const QString&);
    bool showConfirmationMessage(const QString&);
    MessageDisplay::EChoices showChoiseMessage(const QString&);

    enum ENotificationType {
        eTripUpdated,
        eTripListUpdated,
        eFishListUpdated,
        eFishPropertyUpdated,
        eLureListUpdated,
        eLureUpdated,
        ePlaceUpdated,
        ePlaceListUpdated,
        ePlaceInternalUpdate,
        eWayPointsUpdated,
        eStatisticsUpdated,
        eStatisticsEngineUpdated
    };

signals:
    void observerNotification(int);

public slots:

protected:    
    virtual void sendNotificationToObservers(ENotificationType type);
    //This is an abstract class
     explicit Controller(QObject *parent = 0);

private:
     MessageDisplay* display();
     MessageDisplay* m_display;
};

#endif // CONTROLLER_H
