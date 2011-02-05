#include <QDebug>
#include "fish.h"
#include "trollingexception.h"

Fish::Fish():
        m_lure(NULL)
{
    m_stringprops << FISH_MISC_TEXT << FISH_SPECIES <<
            FISH_GETTER << FISH_METHOD;
    m_timeprops << FISH_TIME;
    m_intprops << FISH_WIND << FISH_WEATHER << FISH_PRESSURE <<
            FISH_WIND_DIRECTION << FISH_PRESSURE_CHANGE;
    m_boolprops << FISH_IS_GROUP << FISH_IS_UNDERSIZE <<
            FISH_IS_CATCHRELEASED;
    m_doubleprops << FISH_LENGTH << FISH_WEIGHT <<
            FISH_SPOT_DEPTH << FISH_WATER_TEMP <<
            FISH_AIR_TEMP << FISH_TOTAL_DEPTH <<
            FISH_TROLLING_SPEED << FISH_LINE_WEIGHT <<
            FISH_RELEASE_WIDTH << FISH_COORDINATES_LAT <<
            FISH_COORDINATES_LON;
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

void Fish::setUserField(const QString& p_field, const QString& p_value)
{
    QString userfield = getProperty(FISH_USERFIELD).toString();
    QStringList keyvalues = userfield.split("\n");
    for(int loop=0; loop < keyvalues.count(); loop++)
    {
        if( keyvalues.at(loop).split("=").count() > 1 )
        {
            QString key = keyvalues.at(loop).split("=").at(0);
            if(key == p_field)
            {
                keyvalues[loop] = key+"="+p_value;
                m_properties[FISH_USERFIELD] = keyvalues.join("\n");
                return;
            }
        }
    }
    m_properties[FISH_USERFIELD] = userfield+"\n"+p_field+"="+p_value;
}

QMap<QString, QString> Fish::getUserFields()
{
    QMap<QString, QString> retval;
    QString userfield = getProperty(FISH_USERFIELD).toString();
    QStringList keyvalues = userfield.split("\n");
    for(int loop=0; loop < keyvalues.count(); loop++)
    {
        if( keyvalues.at(loop).split("=").count() > 1 )
        {
            QString key = keyvalues.at(loop).split("=").at(0);
            QString value;
            if(keyvalues.at(loop).split("=").count() == 2)
            {
                value = keyvalues.at(loop).split("=").at(1);
            }
            retval[key] = value;
        }
    }
    return retval;
}

void Fish::setProperty(const QString& p_prop, double p_value)
{
    if(m_doubleprops.contains(p_prop))
        m_properties[p_prop] = p_value;
    else
        throw TrollingException("Dont try to set double value to property: "+p_prop);
}

void Fish::setProperty(const QString& p_prop, QString p_value)
{
    if(m_stringprops.contains(p_prop))
    {
        m_properties[p_prop] = p_value;
    }
    else if(m_doubleprops.contains(p_prop))
    {
        bool bConvOk = false;
        if(p_value.isEmpty())
        {
            m_properties[p_prop] = QString();
        }
        else
        {
            p_value.toDouble(&bConvOk);
            if(bConvOk)
            {
                m_properties[p_prop] = p_value;
            }
            else
            {
                throw TrollingException("Dont try to set double value to property: "+p_prop);
            }
        }
    }
    else
    {
        throw TrollingException("Dont try to set string value to property: "+p_prop);
    }
}

void Fish::setProperty(const QString& p_prop, int p_value)
{
    if(m_intprops.contains(p_prop))
        m_properties[p_prop] = p_value;
    else
        throw TrollingException("Dont try to set int value to property: "+p_prop);
}

void Fish::setProperty(const QString& p_prop, QTime p_value)
{
    if(m_timeprops.contains(p_prop))
        m_properties[p_prop] = p_value;
    else
        throw TrollingException("Dont try to set time value to property: "+p_prop);
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
    if(m_boolprops.contains(p_prop))
        m_properties[p_prop] = p_value;
    else
        throw TrollingException("Dont try to set time value to property: "+p_prop);
}

void Fish::setProperty(const QString& p_prop, QVariant p_value)
{
     m_properties[p_prop] = p_value;
}

QList<QString> Fish::getPropertyNames()
{
    return m_properties.keys();
}
