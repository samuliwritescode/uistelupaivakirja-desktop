#include <QDebug>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include "xmlwriter.h"

XMLWriter::XMLWriter(const QString& p_filename):
        m_filename(p_filename)
{

}

bool XMLWriter::write(TrollingObject* p_object)
{
    if(!loadDocument())
        return false;

    QDomElement trollingObject;
    int id = p_object->getId();

    if(!getTrollingObjectElement(trollingObject, id))
        return false;

    p_object->setId(id);
    clearNodeContents(trollingObject);

    trollingObject.setAttribute("type", p_object->getType());

    QHashIterator<QString, QVariant> iter(p_object->getProperties());
    while( iter.hasNext() )
    {
        iter.next();
        if(iter.value().toString().isEmpty())
            continue;
        QDomElement property = m_document.createElement(iter.key());
        QDomText text = m_document.createTextNode(iter.value().toString());
        property.appendChild(text);
        trollingObject.appendChild(property);
    }

    QList< QHash<QString, QVariant> > list = p_object->getList();
    QDomElement listelement = m_document.createElement("PropertyList");
    for(int loop=0; loop < list.size(); loop++)
    {
        QDomElement listItem = m_document.createElement("PropertyListItem");
        QHash<QString, QVariant> subProperty = list.at(loop);
        QHashIterator<QString, QVariant> iter(subProperty);
        while( iter.hasNext() )
        {
            iter.next();
            if(iter.value().toString().isEmpty())
                continue;
            QDomElement property = m_document.createElement(iter.key());
            QDomText text = m_document.createTextNode(iter.value().toString());
            property.appendChild(text);
            listItem.appendChild(property);
        }
        listelement.appendChild(listItem);
    }
    trollingObject.appendChild(listelement);

    QFile file(m_filename);
    if(!file.open(QIODevice::WriteOnly))
        return false;

    file.write(m_document.toByteArray());
    file.close();
    return true;
}

void XMLWriter::clearNodeContents(QDomNode& p_node)
{
    while( p_node.hasChildNodes() )
    {
        QDomNode node = p_node.childNodes().at(0);
        p_node.removeChild(node);
        node.clear();
    }
}

bool XMLWriter::remove(TrollingObject* p_object)
{
    if(!loadDocument())
        return false;

    QDomElement trollingObject;
    int id = p_object->getId();

    if(!getTrollingObjectElement(trollingObject, id))
        return false;

    clearNodeContents(trollingObject);
    trollingObject.parentNode().removeChild(trollingObject);

    QFile file(m_filename);
    if(!file.open(QIODevice::WriteOnly))
        return false;

    file.write(m_document.toByteArray());
    file.close();
    return true;
}

bool XMLWriter::loadAll(const QString& p_type, TrollingObjectFactory* p_factory, int id /*=-1*/)
{
    if(!loadDocument())
        return false;

    if(m_document.elementsByTagName("TrollingObjects").count() != 1)
    {
        qDebug() << "Invalid number of TrollingObjects roots";
        return false;
    }

    QDomNodeList trollingobjectsnodelist = m_document.elementsByTagName("TrollingObject");

    for(int loop0=0; loop0 < trollingobjectsnodelist.size(); loop0++)
    {
        if(trollingobjectsnodelist.at(loop0).isElement())
        {
            QDomElement trollingobjectelement = trollingobjectsnodelist.at(loop0).toElement();
            QDomNodeList propertiesnodes = trollingobjectelement.childNodes();
            QHash<QString, QVariant> properties;
            if(trollingobjectelement.attribute("type") != p_type)
            {
                qDebug() << "Skipping non expected type" << p_type;
                continue;
            }

            if(id != -1)
            {
                int currId = trollingobjectelement.attribute("id").toInt();
                if(id != currId)
                    continue;
            }

            TrollingObject* object = p_factory->createTrollingObject(p_type);
            object->setId(trollingobjectelement.attribute("id").toInt());

            for(int loop=0; loop < propertiesnodes.size(); loop++)
            {
                QDomNode node = propertiesnodes.at(loop);
                if(node.isElement() && node.toElement().tagName() != "PropertyList")
                {
                    QDomElement element = node.toElement();
                    properties[element.tagName()] = element.text();
                    //qDebug() << "read" << element.tagName();
                }
                else if(node.isElement() && node.hasChildNodes() && node.toElement().tagName() == "PropertyList")
                {
                    QList< QHash<QString, QVariant> > list;
                    QDomNodeList propertylist = node.toElement().elementsByTagName("PropertyListItem");
                    for(int loop2=0; loop2 < propertylist.size(); loop2++)
                    {
                        QHash<QString, QVariant> propertylistitem;
                        QDomNodeList listitemlist = propertylist.at(loop2).childNodes();
                        for(int loop3=0; loop3 < listitemlist.size(); loop3++)
                        {
                            QDomNode subnode = listitemlist.at(loop3);
                            if(subnode.isElement())
                            {
                               QDomElement subelement = subnode.toElement();
                               propertylistitem[subelement.tagName()] = subelement.text();
                               //qDebug() << "subread" << subelement.tagName();
                            }
                        }
                        list.push_back(propertylistitem);
                    }
                    object->storeList(list);
                }
            }
            object->storeProperties(properties);
        }
    }
    return true;
}

bool XMLWriter::getTrollingObjectElement(QDomElement& p_element, int& p_id)
{
    QDomElement root;
    int maxId=1;
    if(m_document.elementsByTagName("TrollingObjects").count() == 0) //document is empty
    {
        root = m_document.createElement("TrollingObjects");
        root.setAttribute("MaxId", QString::number(maxId));
        m_document.appendChild(root);
    }
    else if(m_document.elementsByTagName("TrollingObjects").count() == 1) //document is OK
    {
        QDomNode node = m_document.elementsByTagName("TrollingObjects").at(0);
        if(node.isElement())
        {
            root = node.toElement();
            maxId = root.attribute("MaxId").toInt();
        }
        else
        {
            qDebug() << "TrollingObject is not element";
            return false;
        }
    }
    else //invalid document. Cant read.
    {
        qDebug() << "Invalid count of trollingobjects nodes";
        return false;
    }

    QDomNodeList objects = root.elementsByTagName("TrollingObject");

    for(int loop=0; loop < objects.size(); loop++)
    {
        QDomNode node = objects.at(loop);
        if(node.isElement())
        {
            QDomElement element = node.toElement();
            QString id = element.attribute("id");
            if(id.toInt() == p_id)
            {
                if(maxId < p_id)
                    root.setAttribute("MaxId", QString::number(p_id));
                p_element = element;
                return true;
            }
        }
    }

    QDomElement element = m_document.createElement("TrollingObject");
    maxId += 1;
    element.setAttribute("id", QString::number(maxId));
    root.appendChild(element);
    root.setAttribute("MaxId", QString::number(maxId));
    p_element = element;
    p_id = maxId;
    return true;
}

QList<int> XMLWriter::getIds(const QString& p_type)
{
    QList<int> ids;
    if(!loadDocument())
        return ids;

    QDomNodeList nodelist = m_document.elementsByTagName("TrollingObject");
    for(int loop=0; loop < nodelist.size(); loop++)
    {
        QDomNode trollingnode = nodelist.at(loop);
        if(trollingnode.isElement())
        {
            QDomElement element = trollingnode.toElement();
            if(p_type.isNull() || element.attribute("type") == p_type)
            {
                ids.append(element.attribute("id").toInt());
            }
        }
    }
    return ids;
}

bool XMLWriter::loadDocument()
{
    QFile file(m_filename);
    if(file.open(QIODevice::ReadOnly))
    {
        QString error;
        int errorLine = 0;
        int errorColumn = 0;

        if(!m_document.setContent(&file, &error, &errorLine, &errorColumn))
        {
            qDebug() << "Cant' read from document" << error << errorLine << " " << errorColumn;
            file.close();
            return false;
        }
    }

    file.close();
    return true;
}
