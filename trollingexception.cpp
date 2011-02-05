#include "trollingexception.h"
#include <QDebug>

TrollingException::TrollingException(const QString& p_message)
{
    qCritical() << "TrollingException" << p_message;
}
