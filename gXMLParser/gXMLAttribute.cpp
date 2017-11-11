#include "gXMLParser.h"

using namespace gfw;
int gXMLAttribute::toInt()
{
    return szValue.toInt();
}

float gXMLAttribute::toFloat()
{
    return szValue.toFloat();
}

char *gXMLAttribute::toString()
{
    return szValue.ascii();
}
//Done with attribute


