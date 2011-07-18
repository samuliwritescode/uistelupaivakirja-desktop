#ifndef RELOADINGTROLLINGOBJECTFACTORY_H
#define RELOADINGTROLLINGOBJECTFACTORY_H
#include "trollingobjectfactory.h"
#include "trollingobject.h"

class ReloadingTrollingObjectFactory : public TrollingObjectFactory
{
public:
    ReloadingTrollingObjectFactory();
    void setTarget(TrollingObject* object);
    virtual TrollingObject* createTrollingObject(const QString& p_type);

private:
    TrollingObject* m_target;
};

#endif // RELOADINGTROLLINGOBJECTFACTORY_H
