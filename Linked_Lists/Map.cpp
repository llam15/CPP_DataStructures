#include "Map.h"


Map::Map(){
    // Create an empty map (i.e., one with no key/value pairs)
    m_numPairs = 0; // No pairs
    m_headptr = nullptr; // Empty list
    m_tailptr = nullptr;
}

Map::Map(const Map &oldMap){
    // Create a new map and duplicate an old one
    m_numPairs = 0; // Initialize variables
    m_headptr = nullptr;
    m_tailptr = nullptr;
    
    KV_PAIR *dupl = oldMap.m_headptr;
    while (dupl != nullptr) { // Traverse list and duplicate items
        insert(dupl->key, dupl->val);
        dupl = dupl->next;
    }
    
}

Map::~Map(){
    KV_PAIR *temp = m_headptr; // Start at top of list
    while (temp != nullptr) { // List traversal
        KV_PAIR *next = temp->next; // Pointer to next item (so don't lose list)
        delete temp; // Deallocate memory
        temp = next; // Continue traversal
    }
    
}

Map& Map::operator= (const Map &oldMap){
    if (&oldMap == this) { // Check if parameter is same object
        return (*this);
    }
    
    // Delete existing list
    KV_PAIR *temp = m_headptr; // Start at top of list
    while (temp != nullptr) { // List traversal
        KV_PAIR *next = temp->next; // Pointer to next item (so don't lose list)
        delete temp; // Deallocate memory
        temp = next; // Continue traversal
    }
    
    m_headptr = nullptr;
    m_tailptr = nullptr;
    m_numPairs = 0;
    
    KV_PAIR *dupl = oldMap.m_headptr;
    while (dupl != nullptr) { // Traverse list and duplicate items
        insert(dupl->key, dupl->val);
        dupl = dupl->next;
    }
    
    return (*this);
}

bool Map::empty() const{
    // Return true if the map is empty, otherwise false.
    return m_numPairs == 0;
}

int Map::size() const{
    // Return number of key/value pairs in the map
    return m_numPairs;
}

bool Map::insert(const KeyType& key, const ValueType& value){
    // If key is not equal to any key currently in the map, and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that either the key is already in the map)`
    
    if (!contains(key)) { // If key is not in map
        KV_PAIR *insert = new KV_PAIR; // Dynamically allocate new key/value pair
        insert->key = key; // Set key and value
        insert->val = value;
        insert->next = m_headptr; // Link new node to next node
        insert->prev = nullptr; // No previous node b/c at top of list
        
        if (m_numPairs == 0)
            m_tailptr = insert; // Set tailpointer
        
        if (insert->next != nullptr)
            insert->next->prev = insert; // Set following node's previous pointer to new node
        
        m_headptr = insert; // Set head pointer to new node
        m_numPairs++; // Increase count of number of pairs in map
        
        return true;
    }
    
    return false;
    
}

bool Map::update(const KeyType& key, const ValueType& value){
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // Otherwise, make no change to the map and return false.
    
    if (contains(key)) { // Check if key is in map
        getPair(key)->val = value; // Map key to new value
        return true;
    }
    
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value){
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // If key is not equal to any key currently in the map, and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that the key is not already in the map)
    
    if (!update(key, value))  // If cannot update (key is not in map)
        return insert(key, value); // True if can insert into map
    
    return true;
}

bool Map::erase(const KeyType& key){
    // If key is equal to a key currently in the map, remove the key/value
    // pair with that key from the map and return true.  Otherwise, make
    // no change to the map and return false.
    
    if (contains(key)) {
        KV_PAIR *toDel = getPair(key);
        
        if (m_headptr->key == key) { // If first item in list
            m_headptr = toDel->next; // Set headpointer to next item (or null if empty)
            
            if (toDel->next != nullptr) // If an item follows, then set next item's previous pointer to nullpointer
                toDel->next->prev = nullptr;
            
            delete toDel; // Delete key/value pair
        }
        
        else{ // If not first item in list
            toDel->prev->next = toDel->next; // Set previous item's next to the item after toDel (or null if last item)
            
            if (toDel->next != nullptr)  // If not last item in list
                toDel->next->prev = toDel->prev; // Set next item's prev to the item before toDel
            
            else  // If last item in list
                m_tailptr = toDel->prev; // Set tailpointer to item before toDel
            
            delete toDel; // Delete key/value pair
        }
        
        m_numPairs--; // Decrease item count
        return true;
    }

    return false;

}

bool Map::contains(const KeyType& key) const{
    // Return true if key is equal to a key currently in the map, otherwise return false.
        KV_PAIR *target = m_headptr;
        while (target != nullptr) { // Traverse list
            if (target->key == key) // Return true if equal key
                return true;
            target = target->next;
        }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const{
    // If key is equal to a key currently in the map, set value to the
    // value in the map that that key maps to and return true.  Otherwise,
    // make no change to the value parameter of this function and return
    // false.
    
    if (contains(key)) { // If key is in map
        KV_PAIR *target = getPair(key); // Find key/value pair and set value
        value = target->val;
        
        return true;
    }
    
    return false;

}

bool Map::get(int i, KeyType& key, ValueType& value) const{
    // If 0 <= i < size(), copy into the key and value parameters the
    // key and value of one of the key/value pairs in the map and return
    // true.  Otherwise, leave the key and value parameters unchanged and
    // return false.
    
    
    if (0 <= i && i < size()) { // Check if valid i input
        KV_PAIR *target = m_headptr;
        
        while (target != nullptr && i-- > 0) // Traverse list until target index
            target = target->next;
        
        key = target->key; // Copy key/value to parameters
        value = target->val;
        return true;
    }
    
    return false;

}

void Map::swap(Map& other){
    // Exchange the contents of this map with the other one.

    int temp = m_numPairs; // Swap pair count
    m_numPairs = other.m_numPairs;
    other.m_numPairs = temp;
    
    KV_PAIR *tempHead = m_headptr; // Swap head pointers (lists)
    m_headptr = other.m_headptr;
    other.m_headptr = tempHead;
    
    
    KV_PAIR *tempTail = m_tailptr; // Swap tail pointers
    m_tailptr = other.m_tailptr;
    other.m_tailptr = tempTail;
}

bool combine(const Map& m1, const Map& m2, Map& result){
    bool allClear = true;
    
    Map final; // To avoid aliasing issues, will set result to final at end.
    KeyType str;
    ValueType v;
    
    final = m1; // Add all pairs from m1 to final
    
    for (int i = 0; i < m2.size(); i++) { // For all pairs in m2
        m2.get(i, str, v);
        if (final.contains(str)) { // If double exists
            ValueType check = 0;
            final.get(str, check);
            
            if (check != v) { // Check if not same value
                allClear = false;
                final.erase(str); // Remove pair if differing vaues
            }
        }
        
        else final.insert(str, v); // If not double, add to final
    }
    
    result = final;
    
    return allClear;
}

void subtract(const Map& m1, const Map& m2, Map& result){
    
    Map final; // To avoid aliasing issues. Will set result to final at end
    KeyType str;
    ValueType v;
    
    final = m1; // Add all pairs from m1 to final
    for (int i = 0; i < m2.size(); i++) { // For all pairs in m2
        m2.get(i, str, v);
        if (final.contains(str)) // If double exists, remove it
                final.erase(str);
    }
    
    result = final;

}



