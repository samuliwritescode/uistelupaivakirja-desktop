#include <QDebug>
#include <QtAlgorithms>
#include <math.h>
#include "trollingstatistics.h"
#define OPERATOR_COUNT tr("Määrä")
#define OPERATOR_SUM tr("Summa")
#define OPERATOR_MEAN tr("Keskiarvo")
#define COMPARISON_EQUAL tr("Täsmää")
#define COMPARISON_INCLUDES tr("Sisältää")
#define COMPARISON_GREATER tr("Suurempi")
#define COMPARISON_LESS tr("Pienempi")

TrollingStatistics::TrollingStatistics():
        m_operator(OPERATOR_COUNT),
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

void TrollingStatistics::setOperator(const QString& p_operator)
{
    m_operator = p_operator;
}

QStringList TrollingStatistics::getOperators()
{
    QStringList retval;
    retval << OPERATOR_COUNT;
    retval << OPERATOR_SUM;
    retval << OPERATOR_MEAN;
    return retval;
}

void TrollingStatistics::setOperand(const QString& p_field)
{
    m_field = p_field;
}

void TrollingStatistics::setScaling(bool p_scale)
{
    m_doScaling = p_scale;
}

bool TrollingStatistics::supportOperand()
{
    if(m_operator == OPERATOR_MEAN ||
       m_operator == OPERATOR_SUM)
        return true;

    return false;
}

QHash<QString, QString> TrollingStatistics::calculate(const QList<QHash<QString, QString> >& statistics)
{
    QHash<QString, QString> retval;
    QHash<QString, double> fishcount;
    if(m_operator == OPERATOR_COUNT)
    {
        fishcount = countFields(statistics, m_X);
    }else if(m_operator == OPERATOR_MEAN)
    {
        QHash<QString, double> sum = sumFields(statistics, m_field);
        QHash<QString, double> count = countFields(statistics, m_field);
        for(QHash<QString, double>::iterator iter= count.begin(); iter!=count.end(); iter++)
        {
            fishcount[iter.key()] = sum[iter.key()] / count[iter.key()];
        }
    } else if(m_operator == OPERATOR_SUM)
    {
        fishcount = sumFields(statistics, m_field);
    }

    for(QHash<QString, double>::iterator iter= fishcount.begin(); iter!=fishcount.end(); iter++)
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

QHash<QString, double> TrollingStatistics::countFields(const QList<QHash<QString, QString> >& statistics, const QString& field)
{
    QHash<QString, double> retval;
    for(int loop=0; loop < statistics.count(); loop++)
    {
        QHash<QString, QString> statline = statistics.at(loop);
        if(!statline[field].isEmpty())
        {
            retval[makeGroup(statline[m_X])]++;
        }
    }
    return retval;
}

QHash<QString, double> TrollingStatistics::sumFields(const QList<QHash<QString, QString> >& statistics, const QString& field)
{
    QHash<QString, double> retval;
    for(int loop=0; loop < statistics.count(); loop++)
    {
        QHash<QString, QString> statline = statistics.at(loop);
        retval[makeGroup(statline[m_X])] = retval[makeGroup(statline[m_X])] + statline[field].toDouble();
    }
    return retval;
}

QString TrollingStatistics::getX()
{
    return m_X;
}

QString TrollingStatistics::getZ()
{
    return m_Z;
}

QString TrollingStatistics::getField()
{
    return m_field;
}

QString TrollingStatistics::getOperator()
{
    return m_operator;
}

bool TrollingStatistics::isScaling()
{
    return m_doScaling;
}

TrollingStatisticsTable TrollingStatistics::stats3D()
{
    TrollingStatisticsTable retval;
    QHash<QString, QPair<QString, QString> > filters = m_filters;
    QString y = m_X;
    emit progress(0);

    QHash<QString, QString> allPossible = stats();
    retval.m_columns = allPossible.keys();
    qSort(retval.m_columns);

    //get column values
    m_X = m_Z;
    QHash<QString, QString> cols = stats();
    retval.m_rows = cols.keys();
    qSort(retval.m_rows);
    int idx = 0;
    for(int loop=0; loop < retval.m_rows.count(); loop++)
    {
        QString col = retval.m_rows.at(loop);
        emit progress(100*idx/cols.size());
        m_X = y;
        setFilterComparison(m_Z, COMPARISON_EQUAL);
        setFilterText(m_Z, col);
        TrollingStatisticsTableRow row;
        QHash<QString, QString> stat = stats();

        retval.m_data.push_back(stat);
        m_filters = filters;
        idx++;
    }

    emit progress(100);
    m_X = y;
    m_filters = filters;
    return retval;
}

bool TrollingStatistics::isMatch(const QHash<QString, QString>& p_statline)
{
    for(QHash<QString, QPair<QString, QString> >::iterator iter = m_filters.begin(); iter != m_filters.end(); iter++)
    {
        if(p_statline.contains(iter.key()))
        {
            if(p_statline[iter.key()] != iter.value().first)
            {
                return false;
            }
        }
    }
    return true;
}

void TrollingStatistics::clearFilter()
{
    m_filters.clear();
}

void TrollingStatistics::setFilterComparison(const QString& p_field, const QString& p_compare)
{
    if(m_filters.contains(p_field))
    {
        QPair<QString, QString> filter = m_filters[p_field];
        filter.second = p_compare;
        m_filters[p_field] = filter;
    }
    else
    {
        m_filters[p_field] = QPair<QString, QString>("", p_compare);
    }
}

void TrollingStatistics::setFilterText(const QString& p_field, const QString& p_text)
{
    if(m_filters.contains(p_field))
    {
        QPair<QString, QString> filter = m_filters[p_field];
        filter.first = p_text;
        m_filters[p_field] = filter;
    }
    else
    {
        m_filters[p_field] = QPair<QString, QString>(p_text, "");
    }
}
