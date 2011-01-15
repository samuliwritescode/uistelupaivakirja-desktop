#ifndef TROLLINGOBJECT_H
#define TROLLINGOBJECT_H
#include <QMap>
#include <QString>
#include <QVariant>

class TrollingObject
{
public:
    TrollingObject();
protected:
    QMap<QString, QVariant> m_properties;
};

#endif // TROLLINGOBJECT_H
