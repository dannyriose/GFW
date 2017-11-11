#ifndef GLINKEDLIST_H
#define GLINKEDLIST_H
#include <gBase/gtypes.h>
#include <gMemory/gBaseArray.h>

#ifdef WIN32
#ifdef SHARED_LIBRARY
#ifdef SHARED_EXPORT
#define SHARED_GLINKEDLIST __declspec(dllexport)
#else
#define SHARED_GLINKEDLIST __declspec(dllimport)
#endif
#else
#define SHARED_GLINKEDLIST
#endif
#else
#define SHARED_GLINKEDLIST
#endif
namespace gfw
{

    class SHARED_GLINKEDLIST gLinkedList
    {
        void *d;
    public:
        gLinkedList();
        gLinkedList(const gLinkedList &other);
        ~gLinkedList();
        void create();
        void append( const void *pData);
        void append(const gLinkedList &other);
        void remove(gu32 iElement);
        void remove();
        void *data() const;
        void *data(gu32 index) const;
        void clear();
        void markForRemoval(gu32 index);
        void markForRemoval();
        void removeMarked();
        void select(gu32 index) const;
        void insert(gu32 index,const void *data);
        void moveUpwards(gu32 element);
        void moveDownWards(gu32 element);

        gu32 count() const;
        gu32 count(const void *pData,gUIntArray *indexesOut=0,G_SEARCH_OPERATOR op=G_SEARCH_EQUAL) const;
        bool search(const void *pData,gu32 startPos=0,gu32 *indexOut=0,G_SEARCH_OPERATOR op=G_SEARCH_EQUAL) const;
        bool contains(const void *pData) const;
        void first() const;
        void next() const;
        void last() const;
        int index() const;
        bool isEmpty() const;

        void setFunction(IClean func);

        gLinkedList &operator=(const gLinkedList &other);
        gLinkedList &operator << (const void *data);//same as append
        gLinkedList &operator << (const gLinkedList &other);

        bool isByPass() const;
        void setByPass(bool set);
        void *h() const;


    };
}
#endif // GLINKEDLIST_H
