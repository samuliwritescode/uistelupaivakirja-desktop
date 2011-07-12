#ifndef SIMPLETROLLINGOBJECTFACTORY_H
#define SIMPLETROLLINGOBJECTFACTORY_H
#include "trollingobjectfactory.h"
#include "trollingobject.h"

class SimpleTrollingObjectFactory : public TrollingObjectFactory
{
public:
    SimpleTrollingObjectFactory();

    virtual TrollingObject* createTrollingObject(const QString& p_type);
    void setPushToCollection(QList<TrollingObject*>* collection);

private:
    QList<TrollingObject*>* m_collection;
};

#endif // SIMPLETROLLINGOBJECTFACTORY_H
