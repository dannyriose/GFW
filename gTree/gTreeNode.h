#ifndef GTREENODE_H
#define GTREENODE_H

#include <gfw_global.h>

namespace gfw{
class gTree;
class SHARED_GFW gTreeNode:public gBaseShared
{
public:
    gTreeNode();
    gTreeNode(const gTreeNode &other);
    ~gTreeNode();
    void create(const void *data,gTreeNode *father,gTree *tree, IClean func, gu32 index);
    gTreeNode *addChild(const void *data,IClean func);
    void removeChild();
    void removeChild(gu32 index);
    gu32 childCount() const;

    void setFather(gTreeNode *father);

    gTreeNode *firstChild();
    gTreeNode *lastChild();
    gTreeNode *child(gu32 index);
    gTreeNode *child() const;//selected
    gTreeNode *father() const;
    gTreeNode *nextChild();
    gTree *owner() const;
    void clearChild();

    void clearUserData();
    int nodeIndex() const;
    int childIndex() const;
    void *userdata() const;
    IClean userFunction() const;
    void setUserData(const void *data);
    gTreeNode &operator = (const gTreeNode &other);

    static void childClean(void *data);
    bool hasChilds() const;
    bool childIsNull() const;

};
}
#endif // GTREENODE_H
