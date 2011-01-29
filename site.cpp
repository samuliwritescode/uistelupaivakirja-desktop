#include "site.h"

Site::Site()
{
    setType("site");
}

void Site::setName(const QString& p_value)
{
    set("name", p_value);
}

void Site::setCity(const QString& p_value)
{
    set("city", p_value);
}

void Site::setMiscText(const QString& p_value)
{
    set("misctext", p_value);
}

void Site::setInvisible(bool p_value)
{
    set("invisible", p_value);
}

QString Site::getName()
{
    return get("name").toString();
}

QString Site::getCity()
{
    return get("city").toString();
}

QString Site::getMiscText()
{
    return get("misctext").toString();
}

bool Site::getInvisible()
{
    return get("invisible").toBool();
}
