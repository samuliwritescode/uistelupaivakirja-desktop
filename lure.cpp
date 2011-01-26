#include "lure.h"

Lure::Lure()
{
    setType("lure");
}

void Lure::setMaker(const QString& p_value)
{
    m_properties["maker"] = p_value;
}

void Lure::setModel(const QString& p_value)
{
    m_properties["model"] = p_value;
}

void Lure::setSize(const QString& p_value)
{
    m_properties["size"] = p_value;
}

void Lure::setColor(const QString& p_value)
{
    m_properties["color"] = p_value;
}

void Lure::setFavorite(bool p_value)
{
    m_properties["favorite"] = p_value;
}

QString Lure::getMaker()
{
    return m_properties["maker"].toString();
}

QString Lure::getModel()
{
    return m_properties["model"].toString();
}

QString Lure::getSize()
{
    return m_properties["size"].toString();
}

QString Lure::getColor()
{
    return m_properties["color"].toString();
}

bool Lure::getFavorite()
{
    return m_properties["favorite"].toBool();
}
