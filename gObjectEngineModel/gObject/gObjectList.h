#ifndef GOBJECTLIST_H
#define GOBJECTLIST_H

#include <gObjectEngineModel/gEngineModelList/gEngineModelList.h>
#include <gObjectEngineModel/gObject/gObject.h>
namespace gfw
{

class SHARED_GFW gObjectList: public gEngineModelList<gObject>
{
public:
    gObjectList()
    {

    }
    gObjectList(const gObjectList &other):
        gEngineModelList<gObject>(other)
    {

    }

};

}
#endif // GOBJECTLIST_H
