#include <QFile>
#include "htmlreport.h"
#include "trip.h"
#include "place.h"
#include "fish.h"

bool fishLessThan(const Fish* f1, const Fish* f2)
{
   return f1->getTime() < f2->getTime();
}

HTMLReport::HTMLReport(QObject *parent) :
    QObject(parent)
{
    QFile stylefile(":/res/style.css");
    if(stylefile.open(QIODevice::ReadOnly))
    {
        m_style = stylefile.readAll();
        stylefile.close();
    }
}

QString HTMLReport::getReport(Trip* p_trip)
{
    QString retval;
    retval += "<html><head><style type=\"text/css\">";
    retval += m_style;
    retval += "</style></head><body>";
    retval += parseTrip(p_trip);
    retval += "</body></html>";
    return retval;
}

QString HTMLReport::parseTrip(Trip* p_trip)
{
    if(!p_trip)
        return tr("Ei reissua");

    QString retval;

    if(p_trip->getPlace())
    {
        Place* place = p_trip->getPlace();
        retval += divSection("trip_place",
                             place->getName() +
                             ", "+
                             place->getCity() +
                             ", "+
                             place->getMiscText());
    }
    retval += divSection("trip_date", p_trip->getDate().toString());
    retval += divSection("trip_time",
                         p_trip->getTime().first.toString()+
                         " - " +
                         p_trip->getTime().second.toString()
                         );

    retval += "<p>";
    retval += "<table><tr><td>";
    retval += parseFish(p_trip);
    retval += "</td><td>";
    retval += parseWeather(p_trip);
    retval += "</td></tr></table>";
    return retval;
}

QString HTMLReport::parseFish(Trip* p_trip)
{
    QString retval;
    retval += divSection("trip_heading", tr("Kalatapahtumat"));
    retval += "<table>";

    QList<Fish*> fishes;
    for(int loop=0; loop < p_trip->getFishCount(); loop++)
    {
        Fish* fish = p_trip->getFish(loop);
        if(fish->getType() == Fish::eWeather)
            continue;

        fishes.push_back(fish);
    }

    qSort(fishes.begin(), fishes.end(), fishLessThan);
    foreach(Fish* fish, fishes)
    {
        retval += parseFish(fish);
    }

    retval += "</table>";
    return retval;
}

QString HTMLReport::parseWeather(Trip* p_trip)
{
    QString retval;
    retval += divSection("trip_heading", tr("Säätilamuutokset"));
    retval += "<table>";

    QList<Fish*> weathers;
    for(int loop=0; loop < p_trip->getFishCount(); loop++)
    {
        Fish* fish = p_trip->getFish(loop);
        if(fish->getType() != Fish::eWeather)
            continue;

        weathers.push_back(fish);
    }
    qSort(weathers.begin(), weathers.end(), fishLessThan);
    foreach(Fish* fish, weathers)
    {
        retval += parseWeather(fish);
    }

    retval += "</table>";
    return retval;
}

QString HTMLReport::parseLure(Lure* p_lure)
{
    QString retval;
    if(!p_lure)
        return "-";

    retval += p_lure->getLureType();
    retval += " ";
    retval += p_lure->getMaker();
    retval += " ";
    retval += p_lure->getModel();
    retval += " ";
    retval += p_lure->getColor();
    retval += " ";
    retval += p_lure->getSize();
    return retval;
}

QString HTMLReport::parseFish(Fish* p_fish)
{
    QString retval;
    retval += trtdSection(p_fish->getTime().toString(),
                          "");

    retval += trtdSection(tr("Koordinaatit"),
                          googleMapsCoords(p_fish->getCoordinatesLat(),
                                           p_fish->getCoordinatesLon())
                          );

    retval += trtdSection(tr("Laji"),
                          p_fish->getSpecies());

    retval += trtdSection(tr("Viehe"),
                              parseLure(p_fish->getLure()));

    retval += trtdSection(tr("Kalastustapa"),
                          p_fish->getMethod());

    retval += trtdSection(tr("Saaja"),
                          p_fish->getGetter());

    if(p_fish->isGroup())
    {
        retval += trtdSection(tr("Paino"),
                              norm(
                              QString::number(p_fish->getWeight().toDouble()/p_fish->getGroupAmount()),
                              tr("g")));
    }
    else
    {
        retval += trtdSection(tr("Paino"),
                              norm(p_fish->getWeight(),
                              tr("g")));
    }

    retval += trtdSection(tr("Pituus"),
                          norm(p_fish->getLength(),
                          tr("cm")));

    retval += trtdSection(tr("Ottisyvyys"),
                          norm(p_fish->getSpotDepth(),
                          tr("m")));

    retval += trtdSection(tr("Syvyys"),
                          norm(p_fish->getTotalDepth(),
                          tr("m")));

    retval += trtdSection(tr("Vetonopeus"),
                          norm(p_fish->getTrollingSpeed(),
                          tr("km/h")));

    retval += trtdSection(tr("Painotus"),
                          norm(p_fish->getLineWeight(),
                          tr("g")));

    retval += trtdSection(tr("Vapautuspituus"),
                          norm(p_fish->getReleaseWidth(),
                          tr("m")));

    retval += trtdSection(tr("Muuta"),
                          p_fish->getMiscText());

    if(p_fish->getMediaFiles().count() > 0)
    {
        QString media;
        foreach(QString mediafile, p_fish->getMediaFiles())
        {
            media += "<a href=\"file://"+mediafile+"\">"+mediafile+"</a><br>";
        }

        retval += trtdSection(tr("Mediatiedostot"),
                              media);
    }

    if(p_fish->isGroup())
    {
        for(int loop=0; loop < p_fish->getGroupAmount()-1; loop++)
        {
            retval += retval;
        }
    }
    retval += tdSection("<hr>");
    return retval;
}

QString HTMLReport::parseWeather(Fish* p_fish)
{
    QString retval;
    retval += trtdSection(p_fish->getTime().toString(),
                          "");

    retval += trtdSection(tr("Koordinaatit"),
                          googleMapsCoords(p_fish->getCoordinatesLat(),
                                           p_fish->getCoordinatesLon())
                          );

    retval += trtdSection(tr("Säätila"),
                          p_fish->getHumanReadableWeather());

    retval += trtdSection(tr("Sade"),
                          p_fish->getHumanReadableRain());

    retval += trtdSection(tr("Tuuli"),
                          p_fish->getHumanReadableWind());

    retval += trtdSection(tr("Tuulen suunta"),
                          p_fish->getHumanReadableWindDirection());

    retval += trtdSection(tr("Ilmanpaine"),
                          p_fish->getHumanReadablePressure());

    retval += trtdSection(tr("Ilmanpaineen muutos"),
                          p_fish->getHumanReadablePressureChange());

    retval += trtdSection(tr("Ilman lämpötila"),
                          norm(p_fish->getAirTemp(),
                          tr("C")));

    retval += trtdSection(tr("Veden lämpötila"),
                          norm(p_fish->getWaterTemp(),
                          tr("C")));

    retval += trtdSection(tr("Muuta"),
                          p_fish->getMiscText());

    if(p_fish->getMediaFiles().count() > 0)
    {
        QString media;
        foreach(QString mediafile, p_fish->getMediaFiles())
        {
            media += "<a href=\"file://"+mediafile+"\">"+mediafile+"</a><br>";
        }

        retval += trtdSection(tr("Mediatiedostot"),
                              media);
    }


    return retval;
}

QString HTMLReport::norm(const QString& p_number, const QString& p_unit)
{
    if(p_number.isEmpty())
        return "-";

    if(!p_unit.isEmpty())
        return p_number+" "+p_unit;

    return p_number;
}

QString HTMLReport::divSection(const QString& p_id, const QString& p_content)
{
    QString retval;
    retval += "<div id=\"";
    retval += p_id;
    retval += "\">";
    retval += p_content;
    retval += "</div>\n";
    return retval;
}

QString HTMLReport::tdSection(const QString& p_content)
{
    QString retval;
    retval += "<td>";
    retval += p_content;
    retval += "</td>\n";
    return retval;
}

QString HTMLReport::trtdSection(const QString& p_title, const QString& p_content)
{
    QString retval;
    retval += "<tr>";
    retval += tdSection(p_title);
    retval += tdSection(p_content);
    retval += "</tr>\n";
    return retval;
}

QString HTMLReport::googleMapsCoords(const QString& p_lat, const QString& p_lon)
{
    if(p_lat.isEmpty() || p_lon.isEmpty())
        return "-";

    QString retval;
    retval += "<iframe width=\"250\" height=\"200\" frameborder=\"0\" scrolling=no marginheight=\"0\" marginwidth=\"0\"";
    retval += "src=\"http://maps.google.fi/?ie=UTF8&amp;";
    retval += "ll="+p_lat+","+p_lon;
    retval += "&amp;q="+p_lat+","+p_lon;
    retval += "&amp;spn=0.055964,0.145912&amp;output=embed\">";
    retval += "</iframe><br/><small>";

    return retval;
}
