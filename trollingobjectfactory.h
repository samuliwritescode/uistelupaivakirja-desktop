#ifndef TROLLINGOBJECTFACTORY_H
#define TROLLINGOBJECTFACTORY_H
#include <QString>
class TrollingObject;

class TrollingObjectFactory
{
public:
    virtual TrollingObject* createTrollingObject(const QString& p_type) = 0;
protected:
    TrollingObjectFactory();
};

#endif // TROLLINGOBJECTFACTORY_H
