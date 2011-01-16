#include "trollingobject.h"

TrollingObject::TrollingObject():
        m_id(-1)
{
}

void TrollingObject::storeProperties(QMap<QString, QVariant> p_properties)
{
    m_properties = p_properties;
}

QMap<QString, QVariant> TrollingObject::getProperties()
{
    return m_properties;
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
