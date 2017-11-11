#include "gXMLParser.h"

//Level struct implementation
using namespace gfw;
gXMLLevel::gXMLLevel()
{
    iLevel=LEVEL_UNASIGNED;
    iLevelID=-1;
    tagL=NULL;
}

void gXMLLevel::setLevel(int iLev, const char *sLevel, gNodeTag *Lev)
{
    iLevel=iLev;

    sLevelID=sLevel;
    tagL=Lev;
}

void gXMLLevel::setLevel(int iLvl, int ID,gNodeTag *Lev)
{
    iLevel=iLvl;
    iLevelID=ID;
    tagL=Lev;
}
