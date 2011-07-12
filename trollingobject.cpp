#include <QDebug>
#include "singletons.h"
#include "trollingobject.h"

TrollingObject::TrollingObject():
        m_id(-1),
        m_unsavedChanges(true)
{
}

TrollingObject::~TrollingObject()
{

}

void TrollingObject::storeProperties(QHash<QString, QVariant> p_properties)
{
    m_properties = p_properties;
    m_unsavedChanges = false;
}

void TrollingObject::constructItems(const TrollingObjectItemList&)
{
    qWarning() << "NULL implementation of TrollingObject::constructItems()";
}

TrollingObjectItemList TrollingObject::serializeItems()
{
    qWarning() << "NULL implementation of TrollingObject::serializeItems()";
    return TrollingObjectItemList();
}

void TrollingObject::storeList(TrollingObjectItemList p_list)
{
    constructItems(p_list);
}

void TrollingObject::setSaved()
{
    m_unsavedChanges = false;
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
    if(!m_properties.contains(p_property))
        setUnsaved();
    else if(m_properties[p_property] != p_value)
        setUnsaved();

    m_properties[p_property] = p_value;
}

bool TrollingObject::isUnsaved()
{
    return m_unsavedChanges;
}

QVariant TrollingObject::get(const QString& p_property)
{
    if(m_properties.contains(p_property))
        return m_properties[p_property];
    else
        return QVariant();
}

void TrollingObject::setUnsaved()
{
    m_unsavedChanges = true;
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

QString TrollingObject::importFile(const QString& p_filename)
{
    return Singletons::model()->importFile(this, p_filename);
}

QString TrollingObject::valid()
{
    return QString();
}

bool TrollingObject::operator==(const TrollingObject& other)
{
    TrollingObject& compTo = const_cast<TrollingObject&>(other);
    if(getType() != compTo.getType())
        return false;

    if(getId() != compTo.getId())
        return false;

    if(getProperties() != compTo.getProperties())
        return false;

    if(getList() != compTo.getList())
        return false;

    return true;
}
