#ifndef GTREE_H
#define GTREE_H

#include "gTreeNode.h"
#include <gBase/gtypes.h>
namespace gfw
{

    class SHARED_GFW gTree:public gBaseShared
    {
    public:
        gTree();
        gTree(const gTree &other);
        ~gTree();
        //Creates the tree
        bool create(const void *data, IClean func);
        void addNode(const void *data,IClean func);
        //
        gTreeNode *gotoMain();
        //Navigation through childs
        gTreeNode *selFirstChild(); //! Select first child from current selected node
        gTreeNode *selLastChild(); //! Select last child from current seleted node
        gTreeNode *selChild(gu32 index); //! Select a child node given an index
        gTreeNode *nextChild();//! Selects the next child and returns previous

        int childIndex() const; //! Gets the child index from the current node

        //Moving through nodes
        gTreeNode *gotoChild(); //! the nodepointer to the selected child
        gTreeNode *gotoFather();//! Moves the node pointer to the father node
        gTreeNode *node() const; //! Gets the current node

        void *nodeData() const;//! returns the current node data
        void *childData() const; //! Returns the current child data
        void *fatherData() const;//! returns the current father data
        void *mainData() const; //! returns the main node data

        gu32 childCount() const; //! Gets the the child count from the current selected node

        void deleteNode(); //! removes the current node
        void deleteChildNode(); //! removes the current selected child node
        void setData(const void *data);//! Set the data from the selected node

        gTree &operator= (const gTree &other);
        void copyFrom(gTree &other);
        bool hasChilds() const;
        bool childIsNull() const;

    };
}
#endif // GTREE_H
