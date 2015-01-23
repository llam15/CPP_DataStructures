///*
#include "MultiMap.h"

////////////////////////
// ITERATOR FUNCTIONS //
////////////////////////

MultiMap::Iterator::Iterator()
{
    isValid = false;
}

MultiMap::Iterator::Iterator(treeNode *node, valueNode *val)
{
    // Created at node, so valid = true. current = node
    
    isValid = true;
    m_curr = node;
    m_currVal = val;
}

bool MultiMap::Iterator::valid() const
{
    return isValid;
}

std::string	MultiMap::Iterator::getKey() const
{
    return m_curr->m_key;
}

unsigned int MultiMap::Iterator::getValue()	const
{
    return m_currVal->val;
}

bool MultiMap::Iterator::next()
{
    if (!isValid) { // Invalid, do nothing
        return false;
    }
    
    if (m_currVal->next != nullptr) { // If more values for same key, next value
        m_currVal = m_currVal->next;
        return true;
    }
    
    if (m_curr->right != nullptr) { // If right child exists, move to right
        m_curr = m_curr->right;
        while (m_curr->left != nullptr) { // Move to furthest left of right subtree
            m_curr = m_curr->left;
        }
        m_currVal = m_curr->m_headVal;
        return true;
    }
    
    else { // If no right subchild
        for (;;) {
            if (m_curr->parent == nullptr) { // Last node, return invalid
                return isValid = false;
            }
            
            if (m_curr->parent->left == m_curr) { // Parent w/ poss. right child = next node in in-order traversal.
                m_curr = m_curr->parent;
                m_currVal = m_curr->m_headVal;
                return true;
            }
            
            m_curr = m_curr->parent; // Next in order traversal = parent
        }
    }
    
    return isValid = false;
}

bool MultiMap::Iterator::prev()
{
    if (!isValid) { // Invalid, do nothing
        return false;
    }
    
    if (m_currVal->prev != nullptr) { // If ore values for same key, prev value
        m_currVal = m_currVal->prev;
    }
    
    if (m_curr->left != nullptr) { // If left child exists, move to left
        m_curr = m_curr->left;
        while (m_curr->right != nullptr) { // Move to furthest right of left subtree
            m_curr = m_curr->right;
        }
        m_currVal = m_curr->m_headVal;
        return true;
    }
    
    else { // if no left subchild
        for (; ; ) {
            if (m_curr->parent == nullptr) { // First node, return invalid
                return isValid = false;
            }
            
            if (m_curr->parent->right == m_curr) { // Parent w/ poss. left child = next node in in-order traversal.
                m_curr = m_curr->parent;
                m_currVal = m_curr->m_headVal;
                return true;
            }
            
            m_curr = m_curr->parent; // Next in order traversal = parent
        }
    }
    
    return isValid = false;
}

////////////////////////
// MULTIMAP FUNCTIONS //
////////////////////////

MultiMap::MultiMap()
{
    m_root = nullptr; // Empty tree
}

MultiMap::~MultiMap()
{
    clear(); // Release all dynamically allocated pointers
}

void MultiMap::clear()
{
    freeTree(m_root); // Release all dynamically allocated pointers
}

void MultiMap::freeTree(MultiMap::treeNode *curr)
{
    if (curr == nullptr) { // Have reached end of tree
        return;
    }
    
    freeTree(curr->left); // Free left sub-tree
    freeTree(curr->right); // Free right sub-tree
    
    valueNode* v = curr->m_headVal; // Free all value linked list
    
    while (v != nullptr) {
        valueNode* next = v->next; // Pointer to next value
        delete v; // Deallocate current memory
        v = next; // Continue traversal of list
    }
    
    delete curr;
}

void MultiMap::insert(std::string key, unsigned int value)
{
    if (m_root == nullptr) { // If empty tree, set root
        m_root = new treeNode(key, value, nullptr);
        return;
    }
    
    treeNode *curr = m_root;
    for (;;) {
        if (key == curr->m_key) { // if key already exists, add value to value list
            addValue(curr, value);
            return;
        }
        
        if (key < curr->m_key) { // if key belongs in left subtree
            if (curr->left != nullptr) { // traverse left if left child exists
                curr = curr->left;
            }
            
            else { // add new leaf to left subtree
                curr->left = new treeNode(key, value, curr);
                return;
            }
        }
        
        else if (key > curr->m_key) { // if key belongs in right subtree
            if (curr->right != nullptr) { // traverse right if right child exists
                curr = curr->right;
            }
            
            else { // add new leaf to right subtree
                curr->right = new treeNode(key, value, curr);
                return;
            }
        }
    }
}

void MultiMap::addValue(MultiMap::treeNode *curr, unsigned int value)
{
    valueNode *insert = new valueNode; // Dynamically allocate new key/value pair
    insert->val = value;
    insert->next = curr->m_headVal; // Link new node to next node
    insert->prev = nullptr; // No previous node b/c at top of list
    
    if (insert->next != nullptr)
        insert->next->prev = insert; // Set following node's previous pointer to new node
    
    curr->m_headVal = insert; // Set head pointer to new node
}

MultiMap::Iterator MultiMap::findEqual(std::string	key) const
{
    treeNode *ptr = m_root; // Start at root
    
    while (ptr != nullptr) { // Traverse tree
        if (ptr->m_key == key) { // Found match!
            MultiMap::Iterator it(ptr, ptr->m_headVal);
            return it;
        }
        
        else if (key < ptr->m_key) // Traverse left subtree if key < current key
            ptr = ptr->left;
        
        else
            ptr = ptr->right; // Traverse right subtree. Key > current key
    }
    
    MultiMap::Iterator it; // Did not find key = parameter. Invalid iterator
    return it;
}

MultiMap::Iterator MultiMap::findEqualOrSuccessor(std::string key) const
{
    MultiMap::Iterator invalid;
    if (m_root == nullptr) { // If empty tree, invalid iterator
        return invalid;
    }
    
    treeNode *curr = m_root; // Start at root
    for (;;) {
        if (key == curr->m_key) { // Found match, return valid iterator
            MultiMap::Iterator itr(curr, curr->m_headVal);
            return itr;
        }
        
        if (key < curr->m_key) { // if key's match belongs in left subtree
            if (curr->left != nullptr) { // traverse left if left child exists
                curr = curr->left;
            }
            
            else { // next greater = current (target's parent)
                MultiMap::Iterator itr(curr, curr->m_headVal);
                return itr;
            }
        }
        
        else if (key > curr->m_key) { // if key's match belongs in right subtree
            if (curr->right != nullptr) { // traverse right if right child exists
                curr = curr->right;
            }
            
            else { // Next greater = anscestor. Traverse until come up from left
                for (;;) {
                    if (curr->parent == nullptr) { // Last node, return invalid iterator
                        return invalid;
                    }
                    
                    if (curr->parent->left == curr) { // Next greater = ancestor
                        MultiMap::Iterator itr(curr->parent, curr->parent->m_headVal);
                        return itr;
                    }
                    
                    curr = curr->parent; // Traverse until correct ancestor
                }
            }
        }
    }
}

MultiMap::Iterator MultiMap::findEqualOrPredecessor(std::string key) const
{
    MultiMap::Iterator invalid;
    if (m_root == nullptr) { // If empty tree, invalid iterator
        return invalid;
    }
    
    treeNode *curr = m_root; // Start at root
    for (;;) {
        if (key == curr->m_key) { // Found match, return valid iterator
            MultiMap::Iterator itr(curr, curr->m_headVal);
            return itr;
        }
        
        if (key < curr->m_key) { // if key's match belongs in left subtree
            if (curr->left != nullptr) { // traverse left if left child exists
                curr = curr->left;
            }
            
            else { // next lesser = ancestor. Traverse until come up from right
                for (;;) {
                    if (curr->parent == nullptr) { // Last node, return invalid iterator
                        return invalid;
                    }
                    
                    if (curr->parent->right == curr) { // Next greater = ancestor
                        MultiMap::Iterator itr(curr->parent, curr->parent->m_tailVal);
                        return itr;
                    }
                    
                    curr = curr->parent; // Traverse until correct ancestor
                }
            }
        }
        
        else if (key > curr->m_key) { // if key's match belongs in right subtree
            if (curr->right != nullptr) { // traverse right if right child exists
                curr = curr->right;
            }
            
            else { // Next lesser = current.
                    MultiMap::Iterator itr(curr, curr->m_tailVal);
                    return itr;
                
            }
        }
    }
}


//*/