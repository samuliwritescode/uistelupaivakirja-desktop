#ifndef STATISTICSCONTROLLER_H
#define STATISTICSCONTROLLER_H
#include <QMap>
#include "controller.h"
#include "trollingstatisticstable.h"

class TrollingStatistics;

class StatisticsController : public Controller
{
    Q_OBJECT
public:
    explicit StatisticsController(QObject *parent = 0);
    virtual void textEvent(EUISource, const QString&);
    virtual void intEvent(EUISource, int);
    virtual QString getTextValue(EUISource source);
    QStringList getFields();
    QStringList getNumericFields();
    QStringList getEngines();
    QMap<QString, QString> getStats();
    TrollingStatisticsTable getStats3D();

signals:
    void progress(int);

private:
    TrollingStatistics* m_stats;

};

#endif // STATISTICSCONTROLLER_H
