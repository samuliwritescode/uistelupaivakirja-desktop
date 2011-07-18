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

void Lure::setColorBack(const QString& p_value)
{
    set("color_back", p_value);
}

void Lure::setColorSide(const QString& p_value)
{
    set("color_side", p_value);
}

void Lure::setColorBelly(const QString& p_value)
{
    set("color_belly", p_value);
}

void Lure::setColorClass(const QString& p_value)
{
    set("color_class", p_value);
}

void Lure::setColorContrast(const QString& p_value)
{
    set("color_contrast", p_value);
}

void Lure::setLureType(const QString& p_value)
{
    set("lure_type", p_value);
}

void Lure::setFavorite(bool p_value)
{
    set("favorite", p_value);
}

void Lure::setNotVisible(bool p_value)
{
    set("notvisible", p_value);
}

void Lure::setNickName(const QString& p_value)
{
    set("nickname", p_value);
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

QString Lure::getColorBack()
{
    return get("color_back").toString();
}

QString Lure::getColorSide()
{
    return get("color_side").toString();
}

QString Lure::getColorBelly()
{
    return get("color_belly").toString();
}

QString Lure::getColorClass()
{
    return get("color_class").toString();
}

QString Lure::getColorContrast()
{
    return get("color_contrast").toString();
}

QString Lure::getLureType()
{
    return get("lure_type").toString();
}

bool Lure::getFavorite()
{
    return get("favorite").toBool();
}

bool Lure::getNotVisible()
{
    return get("notvisible").toBool();
}

QString Lure::getNickName()
{
    return get("nickname").toString();
}

QString Lure::toString()
{
    return getMaker()+" "+getModel()+" "+getSize()+" "+getColor();
}
