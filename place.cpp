#include "place.h"

Place::Place()
{
    setType("place");
}

void Place::setName(const QString& p_value)
{
    set("name", p_value);
}

void Place::setCity(const QString& p_value)
{
    set("city", p_value);
}

void Place::setMiscText(const QString& p_value)
{
    set("misctext", p_value);
}

void Place::setInvisible(bool p_value)
{
    set("invisible", p_value);
}

QString Place::getName()
{
    return get("name").toString();
}

QString Place::getCity()
{
    return get("city").toString();
}

QString Place::getMiscText()
{
    return get("misctext").toString();
}

bool Place::getInvisible()
{
    return get("invisible").toBool();
}
