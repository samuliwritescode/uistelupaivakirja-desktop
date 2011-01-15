#ifndef PLACECONTROLLER_H
#define PLACECONTROLLER_H

#include <QObject>
#include <QStringList>

class PlaceController : public QObject
{
    Q_OBJECT
public:
    explicit PlaceController(QObject *parent = 0);
    QStringList getPlaces();

signals:

public slots:

};

#endif // PLACECONTROLLER_H
