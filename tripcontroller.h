#ifndef TRIPCONTROLLER_H
#define TRIPCONTROLLER_H

#include <QObject>
#include <QStringList>
#include <QDate>

class TripController : public QObject
{
    Q_OBJECT
public:
    explicit TripController(QObject *parent = 0);
    QStringList getPlaces();

signals:

public slots:
    void setDate(const QDate& date);
    void setPlace(const QString& place);
    void addTime(int, int);
    void removeTime(int,int);

};

#endif // TRIPCONTROLLER_H
