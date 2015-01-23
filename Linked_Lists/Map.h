
#ifndef __LinkedLists__Map__
#define __LinkedLists__Map__

#include <string>

typedef std::string KeyType;
typedef double ValueType;

class Map
{
public:
    Map();
    Map(const Map &oldMap);
    ~Map();    
    Map& operator= (const Map &oldMap);
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
    
private:
    struct KV_PAIR{ // Struct for each node. Contains key, value, and pointers to next and previous nodes
        KeyType key;
        ValueType val;
        KV_PAIR *next;
        KV_PAIR *prev;
    };
    
    KV_PAIR *m_headptr; // Pointer to first node
    KV_PAIR *m_tailptr; // Pointer to last node
    int m_numPairs; // Number of pairs in the list
    
    KV_PAIR* getPair(KeyType key) const{ // Returns pointer to pair with matching key, else returns null pointer
            KV_PAIR *target = m_headptr;
            while (target != nullptr) { // List traversal
                if (target->key == key) // Return target if matching key
                    return target;
                
                target = target->next;
            }        
        return nullptr;
    };
    
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);


#endif /* defined(__LinkedLists__Map__) */
