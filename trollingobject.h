#ifndef TROLLINGOBJECT_H
#define TROLLINGOBJECT_H
#include <QHash>
#include <QString>
#include <QStringList>
#include <QVariant>
#include "xmlwriter.h"

typedef QHash<QString, QString> TrollingObjectItem;
typedef QList<TrollingObjectItem> TrollingObjectItemList;

class TrollingObject: public QObject
{
    friend class XMLWriter;
    friend class TrollingModel;
    friend class Synchronizer;
public:
    int getId();
    QString getType();
    bool isUnsaved();
    virtual QString valid();
    virtual ~TrollingObject();
    virtual QString toString();
    bool operator==(const TrollingObject& other);

protected:
    QString importFile(const QString& p_filename);
    void setType(const QString& p_type);
    void set(const QString&, QVariant);
    void setUnsaved();
    QVariant get(const QString&);
    QStringList getKeys();

    virtual void constructItems(const TrollingObjectItemList&);
    virtual TrollingObjectItemList serializeItems();

    TrollingObject();

private:
    //XMLWriter will call these
    void storeProperties(QHash<QString, QString> p_properties);
    void storeList(TrollingObjectItemList p_list);
    QHash<QString, QString> getProperties();
    TrollingObjectItemList getList();
    void setId(int p_id);
    void setSaved();

    int m_id;
    QString m_type;
    QHash<QString, QString> m_properties;
    bool m_unsavedChanges;
};

#endif // TROLLINGOBJECT_H
