#ifndef HTMLREPORT_H
#define HTMLREPORT_H

#include <QObject>
class Trip;
class Fish;
class Lure;

class HTMLReport : public QObject
{
    Q_OBJECT
public:
    explicit HTMLReport(QObject *parent = 0);
    QString getReport(Trip*);

signals:

public slots:
private:
    QString parseLure(Lure*);
    QString norm(const QString& p_number, const QString& p_unit = QString());
    QString parseTrip(Trip*);
    QString parseFish(Fish*);
    QString parseFish(Trip*);
    QString parseWeather(Trip*);
    QString parseWeather(Fish*);
    QString googleMapsCoords(const QString&, const QString&);
    QString divSection(const QString&, const QString&);
    QString tdSection(const QString&);
    QString trtdSection(const QString&, const QString&);
    QString m_style;

};

#endif // HTMLREPORT_H
