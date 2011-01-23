#ifndef TROLLINGOBJECT_H
#define TROLLINGOBJECT_H
#include <QMap>
#include <QString>
#include <QVariant>

class TrollingObject
{
public:
    void storeProperties(QMap<QString, QVariant> p_properties);
    void storeList(QList< QMap<QString, QVariant> > p_list);
    QMap<QString, QVariant> getProperties();
    QList< QMap<QString, QVariant> > getList();

    int getId();
    QString getType();
    void setType(const QString& p_type);
    void setId(int p_id);

protected:
    TrollingObject();
    int m_id;
    QString m_type;
    QMap<QString, QVariant> m_properties;
    QList< QMap<QString, QVariant> > m_list;
};

#endif // TROLLINGOBJECT_H
