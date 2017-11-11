#ifndef GHASHMAP_H
#define GHASHMAP_H
#include <gArray/gList.h>
#include <gLinkedList/gtLinkedList.h>
#include <gString/gString.h>
namespace gfw{
    //! Basic hash functions
    template <typename T>
    struct gKeyHash{

        gsize operator()(const T &key,gu32 tableSize) const{
            return (gsize)key % tableSize;
        }
    };
    struct gKeyHashString{
        gsize operator()(const gString &str,gu32 tableSize) const{
            return (gsize)str.hash() % tableSize;
        }
    };

    template <class K, class V, class H = gKeyHashString>
    //! Implements a very simplistic hash map
    class SHARED_GFW gHashMap{
    public:
        struct gHashMapNode{
            K key;
            V value;
            gHashMapNode *next;
            gHashMapNode():next(0){

            }

            gHashMapNode(const K &_key,const V &_value):key(_key),value(_value),next(0){

            }
            ~gHashMapNode(){

            }
            void clear(){
                if(next){
                    next->clear();
                    delete next;
                }
            }
        };

    protected:
        H m_hashFunc;
        gList<gHashMapNode> m_nodes;
    public:
        gHashMap(){

        }
        gHashMap(const gHashMap<K,V,H> &other):m_nodes(other.m_nodes){

        }
        gHashMap &operator = (const gHashMap<K,V,H> &other){
            m_nodes = other.m_nodes;return *this;
        }
        bool alloc(gu32 _size){
            return m_nodes.alloc(_size);
        }
        void clear(){
            m_nodes.clear();
        }
        void set(const K &key, const V& value){
            gu32 hash = m_hashFunc(key,m_nodes.size());
            gHashMapNode *node = m_nodes.pValue(hash);
            gHashMapNode *prev = 0;
            //search the value
            while(node != 0 && node->key != key){
                prev = node;
                node = node->next;
            }
            if(node == 0){
                node = new gHashMapNode(key,value);
                if(prev == 0){
                    m_nodes.setValue(node,hash);
                }else{
                    prev->next = node;
                }
            }else{
                node->value = value;
            }

        }
        V &value(const K &key, bool *bFound = 0){
            gu32 hash=m_hashFunc(key,m_nodes.size());
            gHashMapNode *node = m_nodes.pValue(hash);

            while(node != 0){
                if(node->key == key){
                    if(bFound)*bFound=true;
                    return node->value;
                }
                node = node->next;
            }
            if(bFound)*bFound = false;
            return *(V *)0;
        }
        const V &value(const K &key, bool *bFound = 0) const{
            gu32 hash=m_hashFunc(key,m_nodes.size());
            gHashMapNode *node = m_nodes.pValue(hash);

            while(node != 0){
                if(node->key == key){
                    if(bFound)*bFound=true;
                    return node->value;
                }
                node = node->next;
            }
            if(bFound)*bFound=false;
            return *(const V *)0;
        }
        bool contains(const K &key, V **optvalout = 0) const{
            gu32 hash=m_hashFunc(key,m_nodes.size());
            gHashMapNode *node = m_nodes.pValue(hash);

            while(node != 0){
                if(node->key == key){
                    if(optvalout)*optvalout = &node->value;
                    return true;
                }
                node = node->next;
            }
            return false;
        }
        void remove(const K &key){
            gu32 hash = m_hashFunc(key,m_nodes.size());
            gHashMapNode *node = m_nodes.pValue(hash);
            gHashMapNode *prev = 0;
            //search the value
            while(node != 0 && node->key != key){
                prev = node;
                node = node->next;
            }
            if(node == 0){
               return; //not found
            }else{
                if(prev == 0){
                    //! Automatically removes old value
                    m_nodes.setValue(node->next,hash);
                }else{
                    prev->next=node->next;
                    delete node;
                }
            }
        }

        virtual ~gHashMap(){
            gHashMapNode *node;

            for(gu32 i=0;i<m_nodes.size();i++){
                node = m_nodes.pValue(i);
                if(node){
                    node->clear();
                }
            }

        }

    };


}
#endif // GHASHMAP_H
