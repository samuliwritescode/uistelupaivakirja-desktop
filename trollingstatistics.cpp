#include <QDebug>
#include <math.h>
#include "trollingstatistics.h"

TrollingStatistics::TrollingStatistics():
        m_unit(TrollingStatistics::eCount),
        m_doScaling(false)
{
}

void TrollingStatistics::setY(const QString& p_y)
{
    m_Y = p_y;
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
        fishcount = countFields(statistics, m_Y);
    }else if(m_unit == TrollingStatistics::eFishPerTime)
    {
        QMap<QString, double> count = countFields(statistics, m_Y);
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
            retval[makeGroup(statline[m_Y])]++;
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
        retval[makeGroup(statline[m_Y])] = retval[makeGroup(statline[m_Y])] + statline[field].toDouble();
    }
    return retval;
}

QList<QMap<QString, QString> > TrollingStatistics::stats3D(const QString& p_byCol)
{
    QList<QMap<QString, QString> > retval;
    QMap<QString, QString> filters = m_filters;
    QString y = m_Y;

    //get column values
    m_Y = p_byCol;
    QMap<QString, QString> cols = stats();
    for(QMap<QString, QString>::iterator iter = cols.begin(); iter != cols.end(); iter++)
    {
        m_Y = y;
        m_filters[p_byCol] = iter.key();
        qDebug() << "filter out" << p_byCol << iter.key();
        retval.push_back(stats());
        m_filters = filters;
    }

    m_Y = y;
    m_filters = filters;
    return retval;
}
