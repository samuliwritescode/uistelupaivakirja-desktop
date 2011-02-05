#ifndef TROLLINGSTATISTICS_H
#define TROLLINGSTATISTICS_H
#include <QObject>

class TrollingStatistics: public QObject
{
    Q_OBJECT
public:
    TrollingStatistics();
    QString stats();
};

#endif // TROLLINGSTATISTICS_H
