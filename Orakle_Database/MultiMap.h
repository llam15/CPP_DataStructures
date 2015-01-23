
#ifndef __Orakle__MultiMap__
#define __Orakle__MultiMap__

#include <string>

class	MultiMap
{
private:
    struct valueNode
    {
        unsigned int val;
        valueNode* next = nullptr;
        valueNode* prev = nullptr;
    };
    
    struct treeNode
    {
        treeNode(std::string key, unsigned int val, treeNode *par)
        {
            parent = par;
            m_key = key;
            m_headVal = new valueNode;
            m_headVal->val = val;
            m_tailVal = m_headVal;
        }
        
        treeNode* parent = nullptr;
        treeNode* left = nullptr;
        treeNode* right = nullptr;
        
        std::string m_key;
        valueNode* m_headVal;
        valueNode* m_tailVal;
    };
    
public:
    //	You	must	implement	this	public	nested	MultiMap::Iterator	class
    class	Iterator
    {
    public:
        Iterator();		//	You	must	have	a	default	constructor
        Iterator(treeNode *node, valueNode *val);
        bool	valid()	const;
        std::string	getKey()	const;
        unsigned	int	getValue()	const;
        bool	next();
        bool	prev();
        
    private:
        bool isValid;
        treeNode *m_curr;
        valueNode *m_currVal;
    };
	
    MultiMap();
    ~MultiMap();
    
    void	clear();
    void	insert(std::string	key, unsigned int value);
    
    Iterator	findEqual(std::string key) const;
    Iterator	findEqualOrSuccessor(std::string	key) const;
    Iterator	findEqualOrPredecessor(std::string	key) const;
	
private:
    //	To	prevent	Multimaps	from	being	copied	or	assigned,	declare	these	members
    //	private	and	do	not	implement	them.
    MultiMap(const	MultiMap&	other);
    MultiMap&	operator=(const	MultiMap&	rhs);
    
    treeNode* m_root;
    void freeTree(treeNode *curr);
    void addValue(treeNode *curr, unsigned int value);
    
    
};


#endif /* defined(__Orakle__MultiMap__) */
