#ifndef XMLWRITER_H
#define XMLWRITER_H
#include <QDomElement>
#include "trollingobjectfactory.h"
#include "trollingobject.h"

class XMLWriter
{
public:
    XMLWriter(const QString& p_filename);
    bool write(TrollingObject* p_object);
    bool loadAll(const QString& p_type, TrollingObjectFactory* p_factory, int id = -1);
    bool remove(TrollingObject* p_object);
    QList<int> getIds(const QString& p_type = QString());

protected:
    bool getTrollingObjectElement(QDomElement& p_element, int& p_id);
    void clearNodeContents(QDomNode& p_node);
    bool loadDocument();

private:
    QString m_filename;
    QDomDocument m_document;
};

#endif // XMLWRITER_H
