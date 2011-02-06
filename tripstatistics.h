#ifndef TRIPSTATISTICS_H
#define TRIPSTATISTICS_H
#include "trollingstatistics.h"

class TripStatistics : public TrollingStatistics
{
    Q_OBJECT
public:
    explicit TripStatistics(QObject *parent = 0);
    virtual QMap<QString, QString> stats() ;

signals:

public slots:

};

#endif // TRIPSTATISTICS_H
