#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QDate>

enum EUISource {eTripDate, eTime49, eTime911,
                eTime1114, eTime1418, eTime1823,
                eTime2304, ePlaceText, eWaterTemp,
                eMiscText, eWeatherClear, eWeatherHalfClear,
                eWeatherOvercast, eWeatherRain, eWeatherFog,
                eWindCalm, eWindFaint, eWindModerate,
                eWindBrisk, eWindHard, eSpecies,
                eWeight, eLength, eSpotDepth,
                eUnderSize, eStartTemp, eEndTemp,
                eSaveTrip, eDeleteFish, eNewFish,
                eDeleteTrip, eNewTrip, eMethod};

class Controller : public QObject
{
    Q_OBJECT
public:

    virtual void buttonEvent(EUISource source) = 0;
    virtual void booleanEvent(EUISource source, bool value) = 0;
    virtual void dateEvent(EUISource source, const QDate& value) = 0;
    virtual void textEvent(EUISource source, const QString& value) = 0;
    virtual void intEvent(EUISource source, int value) = 0;

    virtual QDate getDateValue(EUISource source) = 0;
    virtual bool getBooleanValue(EUISource source) = 0;

signals:
    void observerNotification();

public slots:

protected:
    virtual void sendNotificationToObservers();
    //This is an abstract class
     explicit Controller(QObject *parent = 0);

};

#endif // CONTROLLER_H
