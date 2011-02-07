#ifndef FISHSTATISTICS_H
#define FISHSTATISTICS_H
#include "trollingstatistics.h"

class FishStatistics : public TrollingStatistics
{
    Q_OBJECT
public:
    explicit FishStatistics(QObject *parent = 0);
    virtual QMap<QString, QString> stats();
    virtual QStringList getTextFields();
    virtual QStringList getNumericFields();
    virtual QString getName();

signals:

public slots:

};

#endif // FISHSTATISTICS_H
