#include <QDebug>
#include <QtAlgorithms>
#include <math.h>
#include "trollingstatistics.h"
#define OPERATOR_COUNT tr("Määrä")
#define OPERATOR_SUM tr("Summa")
#define OPERATOR_MEAN tr("Keskiarvo")
#define OPERATOR_GREATEST tr("Suurin")
#define OPERATOR_SMALLEST tr("Pienin")
#define COMPARISON_EQUAL tr("Yhtäsuuri")
#define COMPARISON_INCLUDES tr("Sisältää")
#define COMPARISON_GREATER tr("Suurempi")
#define COMPARISON_LESS tr("Pienempi")

bool groupLessThan(const QString g1, const QString g2)
{
   QString val1 = g1.mid(0, g1.indexOf("-", 1));
   QString val2 = g2.mid(0, g2.indexOf("-", 1));
   return val1.toDouble() < val2.toDouble();
}


TrollingStatistics::TrollingStatistics():
        m_operator(OPERATOR_COUNT),
        m_doScaling(true)
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
    retval << OPERATOR_GREATEST;
    retval << OPERATOR_SMALLEST;
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
       m_operator == OPERATOR_SUM ||
       m_operator == OPERATOR_GREATEST ||
       m_operator == OPERATOR_SMALLEST)
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
    } else if(m_operator == OPERATOR_GREATEST)
    {
        fishcount = minMaxField(statistics, m_field, false);
    } else if(m_operator == OPERATOR_SMALLEST)
    {
        fishcount = minMaxField(statistics, m_field, true);
    }

    for(QHash<QString, double>::iterator iter= fishcount.begin(); iter!=fishcount.end(); iter++)
    {
        retval[iter.key()] = QString::number(iter.value());
    }
    return retval;
}

QString TrollingStatistics::makeGroup(const QString& p_value, const QString& p_field)
{
    if(!m_doScaling || !getNumericFields().contains(p_field))
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
        if(!statline[field].isEmpty() && !statline[m_X].isEmpty())
        {
            retval[makeGroup(statline[m_X], m_X)]++;
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
        if(!statline[field].isEmpty() && !statline[m_X].isEmpty())
        {
            retval[makeGroup(statline[m_X], m_X)] = retval[makeGroup(statline[m_X], m_X)] + statline[field].toDouble();
        }
    }
    return retval;
}

QHash<QString, double> TrollingStatistics::minMaxField(const QList<QHash<QString, QString> >& statistics, const QString& field, bool min)
{
    QHash<QString, double> retval;
    for(int loop=0; loop < statistics.count(); loop++)
    {
        QHash<QString, QString> statline = statistics.at(loop);
        if(statline[field].isEmpty() || statline[m_X].isEmpty())
            continue;

        double value = statline[field].toDouble();
        if(!retval.contains(makeGroup(statline[m_X], m_X)))
        {
            retval[makeGroup(statline[m_X], m_X)] = value;
            continue;
        }

        if(min && value < retval[makeGroup(statline[m_X], m_X)])
            retval[makeGroup(statline[m_X], m_X)] = value;

        if(!min && value > retval[makeGroup(statline[m_X], m_X)])
            retval[makeGroup(statline[m_X], m_X)] = value;

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

void TrollingStatistics::sort(QList<QString>* p_list)
{
    if(m_doScaling && getNumericFields().contains(m_X))
        qSort(p_list->begin(), p_list->end(), groupLessThan);
    else
        qSort(*p_list);
}

TrollingStatisticsTable TrollingStatistics::stats3D()
{
    TrollingStatisticsTable retval;
    QHash<QString, QPair<QString, QString> > filters = m_filters;
    QString y = m_X;
    emit progress(0);

    QHash<QString, QString> allPossible = stats();
    retval.m_columns = allPossible.keys();
    sort(&retval.m_columns);

    if(m_Z.isEmpty())
    {
        retval.m_rows.push_back("");
        retval.m_data.push_back(allPossible);
        emit progress(100);
        return retval;
    }

    //get column values
    setX(m_Z);
    QHash<QString, QString> cols = stats();
    retval.m_rows = cols.keys();
    sort(&retval.m_rows);
    setX(y);

    int idx = 0;
    for(int loop=0; loop < retval.m_rows.count(); loop++)
    {
        QString col = retval.m_rows.at(loop);
        emit progress(100*idx/cols.size());

        setFilterComparison(m_Z, "internal");
        setFilterText(m_Z, col);
        QHash<QString, QString> stat = stats();

        retval.m_data.push_back(stat);
        m_filters = filters;
        idx++;
    }

    emit progress(100);
    m_filters = filters;
    return retval;
}

bool TrollingStatistics::isMatch(const QHash<QString, QString>& p_statline)
{
    for(QHash<QString, QPair<QString, QString> >::iterator iter = m_filters.begin(); iter != m_filters.end(); iter++)
    {
        if(p_statline.contains(iter.key()))
        {
            if(iter.value().first.isEmpty())
                continue;

            if(iter.value().second == COMPARISON_EQUAL)
            {
                if(p_statline[iter.key()].toLower() != iter.value().first.toLower())
                {
                    return false;
                }
            } else if(iter.value().second == COMPARISON_INCLUDES)
            {
                if(!p_statline[iter.key()].toLower().contains(iter.value().first.toLower()))
                {
                    return false;
                }
            } else if(iter.value().second == COMPARISON_GREATER)
            {
                if(p_statline[iter.key()].toDouble() <= iter.value().first.toDouble())
                {
                    return false;
                }
            } else if(iter.value().second == COMPARISON_LESS)
            {
                if(p_statline[iter.key()].toDouble() >= iter.value().first.toDouble())
                {
                    return false;
                }
            } else if(iter.value().second == "internal")
            {
                if(makeGroup(p_statline[iter.key()], iter.key()) != makeGroup(iter.value().first, iter.key()))
                {
                    return false;
                }
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

QStringList TrollingStatistics::getTextComparisonOperators()
{
    QStringList retval;
    retval << COMPARISON_EQUAL;
    retval << COMPARISON_INCLUDES;
    retval.sort();
    return retval;
}

QStringList TrollingStatistics::getNumericComparisonOperators()
{
    QStringList retval;
    retval << COMPARISON_EQUAL;
    retval << COMPARISON_GREATER;
    retval << COMPARISON_LESS;
    retval.sort();
    return retval;
}
