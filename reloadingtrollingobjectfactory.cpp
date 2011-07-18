#include "reloadingtrollingobjectfactory.h"

ReloadingTrollingObjectFactory::ReloadingTrollingObjectFactory()
{
    m_target = NULL;
}

void ReloadingTrollingObjectFactory::setTarget(TrollingObject* object)
{
    m_target = object;
}

TrollingObject* ReloadingTrollingObjectFactory::createTrollingObject(const QString&)
{
    return m_target;
}
