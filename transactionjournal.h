#ifndef TRANSACTIONJOURNAL_H
#define TRANSACTIONJOURNAL_H
#include <QObject>
#include <QPair>

class TransactionJournal: public QObject
{
    Q_OBJECT
public:
    explicit TransactionJournal(QObject *parent = 0);
    virtual ~TransactionJournal();
    bool inJournal(int id, const QString& type);
    int numberOfUncommit(const QString& type);

public slots:
    void addJournal(int id, QString type);
    void resetJournal();

private:
    QList<QPair<int, QString> > getJournal();
    void setJournal(const QList<QPair<int, QString> >& journal);

};

#endif // TRANSACTIONJOURNAL_H
