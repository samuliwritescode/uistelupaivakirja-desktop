#ifndef FISHSTATISTICS_H
#define FISHSTATISTICS_H
#include "trollingstatistics.h"

class FishStatistics : public TrollingStatistics
{
    Q_OBJECT
public:
    explicit FishStatistics(QObject *parent = 0);    
    virtual QStringList getTextFields();
    virtual QStringList getNumericFields();
    virtual QStringList getOperators();
    virtual QString getName();

protected:
    virtual QHash<QString, QString> stats();
    virtual QHash<QString, QString> calculate(const QList<QHash<QString, QString> >&);

signals:

public slots:

};

#endif // FISHSTATISTICS_H
