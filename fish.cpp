#include "fish.h"

Fish::Fish():
        m_lure(NULL)
{

}

void Fish::setType(EType type)
{
    m_properties[FISH_TYPE] = type;
}

Fish::EType Fish::getType()
{
    return static_cast<EType>(m_properties[FISH_TYPE].toInt());
}

void Fish::setLure(Lure* p_lure)
{
    m_lure = p_lure;
}


Lure* Fish::getLure()
{
    return m_lure;
}

void Fish::setProperty(const QString& p_prop, double p_value)
{
    m_properties[p_prop] = p_value;
}

void Fish::setProperty(const QString& p_prop, QString p_value)
{
     m_properties[p_prop] = p_value;
}

void Fish::setProperty(const QString& p_prop, int p_value)
{
     m_properties[p_prop] = p_value;
}

QVariant Fish::getProperty(const QString& p_prop)
{
    return m_properties[p_prop];
}

bool Fish::isProperty(const QString& p_prop, QVariant p_compareTo)
{
    return m_properties[p_prop] == p_compareTo;
}

double Fish::getPropertyDouble(const QString& p_prop)
{
    return m_properties[p_prop].toDouble();
}

void Fish::setProperty(const QString& p_prop, bool p_value)
{
    m_properties[p_prop] = p_value;
}

void Fish::setProperty(const QString& p_prop, QVariant p_value)
{
     m_properties[p_prop] = p_value;
}

QList<QString> Fish::getPropertyNames()
{
    return m_properties.keys();
}
