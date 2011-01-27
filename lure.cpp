#include "lure.h"

Lure::Lure()
{
    setType("lure");
}

void Lure::setMaker(const QString& p_value)
{
    set("maker", p_value);
}

void Lure::setModel(const QString& p_value)
{
    set("model", p_value);
}

void Lure::setSize(const QString& p_value)
{
    set("size", p_value);
}

void Lure::setColor(const QString& p_value)
{
    set("color", p_value);
}

void Lure::setFavorite(bool p_value)
{
    set("favorite", p_value);
}

QString Lure::getMaker()
{
    return get("maker").toString();
}

QString Lure::getModel()
{
    return get("model").toString();
}

QString Lure::getSize()
{
    return get("size").toString();
}

QString Lure::getColor()
{
    return get("color").toString();
}

bool Lure::getFavorite()
{
    return get("favorite").toBool();
}
