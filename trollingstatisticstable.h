#ifndef TROLLINGSTATISTICSTABLE_H
#define TROLLINGSTATISTICSTABLE_H
#include <QString>
#include <QList>
#include <QHash>
#include <QMap>

typedef QHash<QString, QString> TrollingStatisticsTableRow;

class TrollingStatisticsTable
{
public:
    TrollingStatisticsTable();
    QList<QString> m_columns;
    QList<QString> m_rows;
    QList<TrollingStatisticsTableRow> m_data;
};

#endif // TROLLINGSTATISTICSTABLE_H
