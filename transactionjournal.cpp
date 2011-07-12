#include "transactionjournal.h"
#include <QSettings>

TransactionJournal::TransactionJournal(QObject *parent):
    QObject(parent)
{
}

TransactionJournal::~TransactionJournal()
{

}

int TransactionJournal::numberOfUncommit(const QString& type)
{
    return getJournal().count();
}

void TransactionJournal::resetJournal()
{
    QList<QPair<int, QString> > journal;
    setJournal(journal);
}

void TransactionJournal::addJournal(int id, QString type)
{
    if(inJournal(id, type))
        return;

    QList<QPair<int, QString> > journal = getJournal();
    QPair<int, QString> item;
    item.first = id;
    item.second = type;
    journal.append(item);
    setJournal(journal);
}

QList<QPair<int, QString> > TransactionJournal::getJournal()
{
    QList<QPair<int, QString> > journal;
    QSettings settings;
    int size = settings.beginReadArray("TransactionJournal");
    for(int loop=0; loop < size; loop++)
    {
        settings.setArrayIndex(loop);
        QPair<int, QString> item;
        item.first = settings.value("id").toInt();
        item.second = settings.value("type").toString();
        journal.append(item);
    }
    settings.endArray();
    return journal;
}

void TransactionJournal::setJournal(const QList<QPair<int, QString> >& journal)
{
    QSettings settings;
    settings.beginWriteArray("TransactionJournal");
    for(int loop=0; loop < journal.size(); loop++)
    {
        settings.setArrayIndex(loop);
        QPair<int, QString> item = journal.at(loop);
        settings.setValue("id", QString::number(item.first));
        settings.setValue("type", item.second);
    }
    settings.endArray();
}

bool TransactionJournal::inJournal(int id, const QString& type)
{
    QList<QPair<int, QString> > journal = getJournal();
    QPair<int, QString> item;
    item.first = id;
    item.second = type;
    return journal.contains(item);
}
