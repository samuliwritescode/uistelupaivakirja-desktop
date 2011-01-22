#ifndef XMLWRITER_H
#define XMLWRITER_H
#include <QDomElement>
#include "trollingobject.h"

class XMLWriter
{
public:
    XMLWriter(const QString& p_filename);
    bool write(TrollingObject* p_object);
    bool load(TrollingObject* p_object, int p_id);
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
