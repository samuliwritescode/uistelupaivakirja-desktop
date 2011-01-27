#ifndef TROLLINGOBJECT_H
#define TROLLINGOBJECT_H
#include <QHash>
#include <QString>
#include <QVariant>

class TrollingObject
{
public:
    virtual void storeProperties(QHash<QString, QVariant> p_properties);
    virtual void storeList(QList< QMap<QString, QVariant> > p_list);
    virtual QHash<QString, QVariant> getProperties();
    virtual QList< QMap<QString, QVariant> > getList();

    int getId();
    QString getType();
    void setType(const QString& p_type);
    void setId(int p_id);

protected:
    TrollingObject();
    int m_id;
    QString m_type;
    QHash<QString, QVariant> m_properties;
    QList< QMap<QString, QVariant> > m_list;
};

#endif // TROLLINGOBJECT_H
