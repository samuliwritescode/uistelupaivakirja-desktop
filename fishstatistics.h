#ifndef FISHSTATISTICS_H
#define FISHSTATISTICS_H
#include "trollingstatistics.h"

class FishStatistics : public TrollingStatistics
{
    Q_OBJECT
public:
    explicit FishStatistics(QObject *parent = 0);
    virtual QMap<QString, QString> stats();

signals:

public slots:

};

#endif // FISHSTATISTICS_H
