#include <QDebug>
#include <math.h>
#include "trollingstatistics.h"

TrollingStatistics::TrollingStatistics():
        m_unit(TrollingStatistics::eCount),
        m_doScaling(false)
{
}

void TrollingStatistics::setX(const QString& p_y)
{
    m_X = p_y;
}

void TrollingStatistics::setZ(const QString& p_z)
{
    m_Z = p_z;
}

void TrollingStatistics::setUnit(EUnit p_unit)
{
    m_unit = p_unit;
}

void TrollingStatistics::setUnitField(const QString& p_field)
{
    m_field = p_field;
}

void TrollingStatistics::setScaling(bool p_scale)
{
    m_doScaling = p_scale;
}

QMap<QString, QString> TrollingStatistics::calculate(const QList<QMap<QString, QString> >& statistics)
{
    QMap<QString, QString> retval;
    QMap<QString, double> fishcount;
    if(m_unit == TrollingStatistics::eCount)
    {
        fishcount = countFields(statistics, m_X);
    }else if(m_unit == TrollingStatistics::eFishPerTime)
    {
        QMap<QString, double> count = countFields(statistics, m_X);
        QMap<QString, double> time = sumFields(statistics, tr("Aikaa per kala"));
        for(QMap<QString, double>::iterator iter= count.begin(); iter!=count.end(); iter++)
        {
            fishcount[iter.key()] = count[iter.key()] / time[iter.key()];
        }
    }else if(m_unit == TrollingStatistics::eMean)
    {
        QMap<QString, double> sum = sumFields(statistics, m_field);
        QMap<QString, double> count = countFields(statistics, m_field);
        for(QMap<QString, double>::iterator iter= count.begin(); iter!=count.end(); iter++)
        {
            fishcount[iter.key()] = sum[iter.key()] / count[iter.key()];
        }
    } else if(m_unit == TrollingStatistics::eSum)
    {
        fishcount = sumFields(statistics, m_field);
    }

    for(QMap<QString, double>::iterator iter= fishcount.begin(); iter!=fishcount.end(); iter++)
    {
        retval[iter.key()] = QString::number(iter.value());
    }
    return retval;
}

QString TrollingStatistics::makeGroup(const QString& p_value)
{
    if(!m_doScaling)
        return p_value;

    bool bCanConvert = false;
    double value = abs(p_value.toDouble(&bCanConvert));

    if(!bCanConvert)
        return p_value;

    int log = log10(value);
    int start = floor(value/pow(10,log))*pow(10,log);
    int end = start+pow(10,log);

    QString neg;
    if(p_value.toDouble() < 0)
        neg = "-";

    if(p_value.toDouble() != 0)
        return neg+QString::number(start)+"-"+neg+QString::number(end);
    else
        return "0";
}

QMap<QString, double> TrollingStatistics::countFields(const QList<QMap<QString, QString> >& statistics, const QString& field)
{
    QMap<QString, double> retval;
    for(int loop=0; loop < statistics.count(); loop++)
    {
        QMap<QString, QString> statline = statistics.at(loop);
        if(!statline[field].isEmpty())
        {
            retval[makeGroup(statline[m_X])]++;
        }
    }
    return retval;
}

QMap<QString, double> TrollingStatistics::sumFields(const QList<QMap<QString, QString> >& statistics, const QString& field)
{
    QMap<QString, double> retval;
    for(int loop=0; loop < statistics.count(); loop++)
    {
        QMap<QString, QString> statline = statistics.at(loop);
        retval[makeGroup(statline[m_X])] = retval[makeGroup(statline[m_X])] + statline[field].toDouble();
    }
    return retval;
}

TrollingStatisticsTable TrollingStatistics::stats3D()
{
    //QList<QMap<QString, QString> > retval;
    TrollingStatisticsTable retval;
    QMap<QString, QString> filters = m_filters;
    QString y = m_X;

    QMap<QString, QString> allPossible = stats();
    retval.m_columns = allPossible.keys();

    //get column values
    m_X = m_Z;
    QMap<QString, QString> cols = stats();
    retval.m_rows = cols.keys();
    int idx = 0;
    for(QMap<QString, QString>::iterator iter = cols.begin(); iter != cols.end(); iter++)
    {
        qDebug() << (double)idx/cols.size();
        m_X = y;
        m_filters[m_Z] = iter.key();
        TrollingStatisticsTableRow row;
        QMap<QString, QString> stat = stats();
        for(QMap<QString, QString>::iterator i2 = stat.begin(); i2 != stat.end(); i2++)
        {
            row[i2.key()] = i2.value();
        }

        retval.m_data.push_back(row);
        m_filters = filters;
        idx++;
    }

    m_X = y;
    m_filters = filters;
    return retval;
}
