#ifndef TROLLINGSTATISTICS_H
#define TROLLINGSTATISTICS_H
#include <QObject>
#include <QMap>

class TrollingStatistics: public QObject
{
    Q_OBJECT
public:
    enum EUnit { eCount, eFishPerTime, eMean };
    TrollingStatistics();
    QMap<QString, QString> stats();
    void setY(const QString&);
    void setUnit(EUnit);

private:
    QMap<QString, double> countFields(const QList<QMap<QString, QString> >&, const QString&);
    QMap<QString, double> sumFields(const QList<QMap<QString, QString> >&, const QString&);
    QString m_Y;
    EUnit m_unit;
};

#endif // TROLLINGSTATISTICS_H
