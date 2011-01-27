#include <QDebug>
#include "trollingobject.h"

TrollingObject::TrollingObject():
        m_id(-1)
{
}

TrollingObject::~TrollingObject()
{

}

void TrollingObject::storeProperties(QHash<QString, QVariant> p_properties)
{
    m_properties = p_properties;
}

void TrollingObject::constructItem(const TrollingObjectItem&)
{
    qWarning() << "NULL implementation of TrollingObject::constructItem()";
}

TrollingObjectItemList TrollingObject::serializeItems()
{
    qWarning() << "NULL implementation of TrollingObject::serializeItems()";
    return TrollingObjectItemList();
}

void TrollingObject::storeList(TrollingObjectItemList p_list)
{
    foreach(TrollingObjectItem item, p_list)
    {
        constructItem(item);
    }
}

QHash<QString, QVariant> TrollingObject::getProperties()
{
    return m_properties;
}

TrollingObjectItemList TrollingObject::getList()
{
    return serializeItems();
}

int TrollingObject::getId()
{
   return m_id;
}

QString TrollingObject::getType()
{
    return m_type;
}

void TrollingObject::setType(const QString& p_type)
{
    m_type = p_type;
}

void TrollingObject::setId(int p_id)
{
    m_id = p_id;
}

void TrollingObject::set(const QString& p_property, QVariant p_value)
{
    m_properties[p_property] = p_value;
}

QVariant TrollingObject::get(const QString& p_property)
{
    if(m_properties.contains(p_property))
        return m_properties[p_property];
    else
        return QVariant();
}

QStringList TrollingObject::getKeys()
{
    QStringList retval;
    for(QHash<QString, QVariant>::Iterator iter = m_properties.begin(); iter != m_properties.end(); iter++)
    {
        retval << iter.key();
    }
    return retval;
}
