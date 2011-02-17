#include "trollingexception.h"
#include <QDebug>

TrollingException::TrollingException(const QString& p_message)
{
    qCritical() << "TrollingException" << p_message;
    m_message = p_message;
}

QString TrollingException::toString()
{
    return m_message;
}
