#ifndef TRIPCONTROLLER_H
#define TRIPCONTROLLER_H

#include <QObject>
#include <QStringList>
#include <QDate>
#include "trip.h"

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
                eDeleteTrip, eNewTrip};

class TripController : public QObject
{
    Q_OBJECT
public:
    explicit TripController(QObject *parent = 0);

signals:

public:
    void buttonEvent(EUISource source);

    void booleanEvent(EUISource source, bool value);
    void dateEvent(EUISource source, const QDate& value);
    void textEvent(EUISource source, const QString& value);
    void intEvent(EUISource source, int value);

private:
    Trip* m_trip;

};

#endif // TRIPCONTROLLER_H
