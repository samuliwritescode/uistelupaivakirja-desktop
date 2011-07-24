#include "alternative.h"

Alternative::Alternative(const QString& type, const QString& value)
{
    setType("spinneritem");
    set("type", type);
    set("value", value);
}
