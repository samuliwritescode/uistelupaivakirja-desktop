#ifndef TROLLINGEXCEPTION_H
#define TROLLINGEXCEPTION_H
#include <QString>

class TrollingException
{
public:
    TrollingException(const QString&);
    QString toString();
private:
    QString m_message;
};

#endif // TROLLINGEXCEPTION_H
