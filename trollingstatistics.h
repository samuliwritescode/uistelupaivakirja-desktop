#ifndef TROLLINGSTATISTICS_H
#define TROLLINGSTATISTICS_H
#include <QObject>
#include <QMap>
#include "singletons.h"
#include "trollingstatisticstable.h"

class TrollingStatistics: public QObject
{
    Q_OBJECT
public:
    TrollingStatistics();

    virtual QHash<QString, QString> stats() = 0;
    virtual QStringList getTextFields() = 0;
    virtual QStringList getNumericFields() = 0;
    virtual QStringList getOperators();
    virtual QString getName() = 0;
    virtual bool supportOperand();
    TrollingStatisticsTable stats3D();
    void setX(const QString&);
    void setZ(const QString&);
    void setOperand(const QString&);
    void setOperator(const QString&);
    void setScaling(bool);

signals:
    void progress(int);

protected:
    virtual QHash<QString, QString> calculate(const QList<QHash<QString, QString> >&);
    QHash<QString, QString> m_filters;
    QHash<QString, double> countFields(const QList<QHash<QString, QString> >&, const QString&);
    QHash<QString, double> sumFields(const QList<QHash<QString, QString> >&, const QString&);
    QString makeGroup(const QString&);
    QString getX();
    QString getZ();
    QString getField();
    QString getOperator();
    bool isScaling();

private:
    QString m_X;
    QString m_Z;
    QString m_field;
    QString m_operator;
    bool m_doScaling;
};

#endif // TROLLINGSTATISTICS_H
