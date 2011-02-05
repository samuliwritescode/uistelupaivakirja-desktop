#ifndef STATISTICSCONTROLLER_H
#define STATISTICSCONTROLLER_H
#include "controller.h"

class StatisticsController : public Controller
{
    Q_OBJECT
public:
    explicit StatisticsController(QObject *parent = 0);
    virtual QString getTextValue(EUISource source);

signals:

public slots:

};

#endif // STATISTICSCONTROLLER_H
