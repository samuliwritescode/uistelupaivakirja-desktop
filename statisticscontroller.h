#ifndef STATISTICSCONTROLLER_H
#define STATISTICSCONTROLLER_H
#include "controller.h"
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

signals:

public slots:

private:
    TrollingStatistics* m_stats;

};

#endif // STATISTICSCONTROLLER_H
