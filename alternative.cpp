#include "alternative.h"

Alternative::Alternative(const QString& type, const QString& value)
{
    setType("spinneritems");
    set("type", type);
    set("value", value);
}
