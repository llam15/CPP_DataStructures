#ifndef __Orakle__Database__
#define __Orakle__Database__

#include <vector>
#include <string>

class MultiMap;

class Database {
public:
    enum IndexType { it_none, it_indexed };
    
    enum OrderingType { ot_ascending, ot_descending };
    
    struct FieldDescriptor {
        std::string name;
        IndexType index;
    };
    
    struct SearchCriterion {
        std::string fieldName;
        std::string minValue;
        std::string maxValue;
    };
    
    struct SortCriterion {
        std::string fieldName;
        OrderingType ordering;
    };
    
    static const int ERROR_RESULT = -1;
    
    Database();
    
    ~Database();
    
    bool specifySchema(const std::vector<FieldDescriptor>& schema);
    
    bool addRow(const std::vector<std::string>& rowOfData);
    
    bool loadFromURL(std::string url);
    
    bool loadFromFile(std::string filename);
    
    int getNumRows() const;
    
    bool getRow(int rowNum, std::vector<std::string>& row) const;
    
    int search(const std::vector<SearchCriterion>& searchCriteria,
               const std::vector<SortCriterion>& sortCriteria,
               std::vector<int>& results);
    
private:
    // To prevent Databases from being copied or assigned, declare these members // private and do not implement them.
    Database(const Database& other);
    Database& operator=(const Database& rhs);
    
    void clearDatabase();
    
    bool importData(std::string data);
    
    void sort(std::vector<int> &res, int start, int end, const std::vector<SortCriterion> &sortCriteria);
    
    int split(std::vector<int> &a, int first, int last, const std::vector<SortCriterion> &sortCriteria);
    
    bool comesFirst(int &rowA, int &rowB, const std::vector<SortCriterion> &sortCriteria, int rowIndex = 0, int sortIndex = 0);
    
    std::vector<std::vector<std::string> > m_rows;
    
    std::vector<MultiMap*> m_field;
    
    std::vector<FieldDescriptor> m_schema;


};

#endif /* defined(__Orakle__Database__) */
