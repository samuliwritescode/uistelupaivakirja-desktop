#include "trollingobject.h"

TrollingObject::TrollingObject():
        m_id(-1)
{
}

void TrollingObject::storeProperties(QHash<QString, QVariant> p_properties)
{
    m_properties = p_properties;
}

void TrollingObject::storeList(QList< QMap<QString, QVariant> > p_list)
{
    m_list = p_list;
}

QHash<QString, QVariant> TrollingObject::getProperties()
{
    return m_properties;
}

QList< QMap<QString, QVariant> > TrollingObject::getList()
{
    return m_list;
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
