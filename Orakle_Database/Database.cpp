#include "Database.h"
#include "MultiMap.h"
#include "http.h"
#include "Tokenizer.h"
#include<fstream>
#include<unordered_set>


using namespace std;

Database::Database()
{
}

Database::~Database()
{
    clearDatabase(); // Release dynamically allocated memory
}

bool Database::specifySchema(const vector<FieldDescriptor>& schema)
{
    m_schema = schema; // Set schema

    for (int i = 0; i < schema.size(); i++) {
        if (schema[i].index == it_indexed) {
            m_field.push_back(new MultiMap); // Add MultiMap for indexed fields
        }
        else m_field.push_back(nullptr); // Nullpointer for nonindexed fields
    }
    
    if (m_field.size() == 0) { // If no indexed fields in schema, return false
        m_schema.clear();
        return false;
    }
    
    return true;
}

bool Database::addRow(const vector<string>& rowOfData)
{
    // If no schema, or row does not match schema, return false;
    if (m_schema.size() == 0 || rowOfData.size() != m_schema.size()) {
        return false;
    }
    
    m_rows.push_back(rowOfData); // Add row of data
    
    
    for (int i = 0; i<m_field.size(); i++) { // Put fields to be indexed into map
            if (m_field[i] != nullptr) {
                m_field[i]->insert(rowOfData[i], getNumRows()-1);
            }
    }
    
    return true;
}

bool Database::loadFromURL(string url)
{
    string data;
    
    if (HTTP().get(url, data)) // Get input from URL
        return importData(data);
    
    // Import data failed, clear all. Return false
    clearDatabase();
    return false;
}

bool Database::loadFromFile(string filename)
{
    // Get input from file
    ifstream ifs;
    ifs.open(filename.c_str(), ios::in);
    
    // If could not open file
    if (!ifs)
    {
        // Import data failed, clear all. Return false
        clearDatabase();
        return false;
    }
    
    char line[9999];
    string data;
    
    // Create string of data to tokenize
    while (ifs.getline(line, 9999)) {
        data += line;
        data += "\n";
    }
    
    ifs.close();
    
    return importData(data);
}

bool Database::importData(string data)
{
    clearDatabase(); // Delete any previous databases
    
    string lineDelimiter = "\n";
    
    Tokenizer t(data, lineDelimiter); // Get lines/rows
    
    string schemaLine;
    t.getNextToken(schemaLine); // Get first line (schema)
    
    string r;
    vector<string> rows;
    while (t.getNextToken(r)) { // Get all rows
        rows.push_back(r);
    }
    
    string itemDeliminter = ",";
    
    Tokenizer s(schemaLine, itemDeliminter);  // Get field descriptors
    string field;
    vector<FieldDescriptor> schema;
    while (s.getNextToken(field)) { // Set index type/name of field descriptors
        FieldDescriptor f;
        f.name = field;
        
        if (field[field.length()-1] == '*') { // If marked with *, then index
            f.name = f.name.substr(0,f.name.length()-1);
            f.index = it_indexed;
        }
        
        else f.index = it_none;
        
        schema.push_back(f);
    }
    specifySchema(schema); // Add schema to database
    
    for (int i = 0 ; i < rows.size(); i++) { // For all rows
        Tokenizer rT(rows[i], itemDeliminter);
        string info;
        vector<string> curRow;
        while (rT.getNextToken(info)) { // Get each field, add to current row
            curRow.push_back(info);
        }
        
        if (!addRow(curRow)) { // Attempt add row to database
            m_field.clear(); // Import data failed, clear all. Return false
            m_schema.clear();
            m_rows.clear();
            return false;
        }
    }
    
    return true; // Import success
}

int Database::getNumRows() const
{
    return m_rows.size();
}

bool Database::getRow(int rowNum, vector<string>& row) const
{
    if (rowNum < 0 || rowNum >=m_rows.size()) { // Return false if invalid row number
        return false;
    }
    
    row = m_rows[rowNum]; // Set parameter to row at indicated row number
    return true;
}

int Database::search(const vector<SearchCriterion>& searchCriteria,
                     const vector<SortCriterion>& sortCriteria,
                     vector<int>& results)
{
    if (searchCriteria.size() == 0) { // No search criteria, return error
        results.clear();
        return ERROR_RESULT;
    }
    
    // Results of one criteria
    unordered_set<int> matches;
    
    // Check with rest of criteria
    unordered_set<int> check;

    for (int i = 0; i < searchCriteria.size(); i++) {
        // If no min and no max, return error
        if ((searchCriteria[i].minValue == "" && searchCriteria[i].maxValue == "")) {
            results.clear();
            return ERROR_RESULT;
        }
        
        // Find index in schema
        int index =-1;
        for (int j = 0; j < m_schema.size(); j++) {
            if (searchCriteria[i].fieldName == m_schema[j].name) {
                index = j;
                break;
            }
        }
        if (index == -1 || m_field[index] == nullptr) { // Search criteria not in schema, return error
            results.clear();
            return ERROR_RESULT;
        }
        
        MultiMap::Iterator first, last;
        
        // Find first item greater than or equal to minValue
        first = m_field[index]->findEqualOrSuccessor(searchCriteria[i].minValue);
        
        // Find last item less than or equal to maxValue, if max Value exists
        if (searchCriteria[i].maxValue != "") {
            last = m_field[index]->findEqualOrPredecessor(searchCriteria[i].maxValue);
            
            // Attempt insert all items between first and last, inclusive, into matches
            while (first.getKey() <= last.getKey() && first.valid()) {
                
                // First criteria, add to match
                if (i == 0) {
                    matches.insert(first.getValue());
                }
                
                // If insertion fail, then duplicate (matches both criteria), insert to check
                else if (!matches.insert(first.getValue()).second)
                    check.insert(first.getValue());
                
                first.next();
            }
        }
        
        else {
            // No maxValue, attempt insert all items from first to end into matches
            while (first.valid()) {
                // First criteria, add to match
                if (i == 0) {
                    matches.insert(first.getValue());
                }
                
                // If insertion fail, then duplicate (matches both criteria), insert to check
                else if (!matches.insert(first.getValue()).second)
                    check.insert(first.getValue());
                
                first.next();
            }
        }
        
        if (i != 0){
            // Set matches to check. Check contains all duplicates (matches both criteria)
            matches = check;
            
            // Clear check for next criteria
            check.clear();
        }
    }
    
    // Push final results to result vector
    unordered_set<int>::iterator itr = matches.begin();
    for (; itr != matches.end(); itr++) {
        results.push_back(*itr);
    }    
    
    // SORT
    if (sortCriteria.size() != 0) {
        // Check if sort criterias are valid
        for (int i = 0; i < sortCriteria.size(); i++) {
            int rowIndex = -1;
            for (int j = 0; j < m_schema.size(); j++) {
                if (sortCriteria[i].fieldName == m_schema[j].name) {
                    rowIndex = j;
                    break;
                }
            }
            
            if (rowIndex == -1) { // Sort criteria not in schema, return error
                results.clear();
                return ERROR_RESULT;
            }
        }
        
        // Quicksort
        sort(results, 0, results.size()-1, sortCriteria);
    }
    
    
    // Return number of matches found
    return results.size();
}

bool Database::comesFirst(int &rowA, int &rowB, const vector<SortCriterion> &sortCriteria, int rowIndex, int sortIndex)
{
    // Find index in schema
    for (int j = 0; j < m_schema.size(); j++) {
        if (sortCriteria[sortIndex].fieldName == m_schema[j].name) {
            rowIndex = j;
            break;
        }
    }
    
    // Last criteria, final
    if (sortIndex == sortCriteria.size()-1) {
        // If ascending, true if a < b. If descending, true if a > b
        if (sortCriteria[sortIndex].ordering == ot_ascending)
            return m_rows[rowA][rowIndex] <= m_rows[rowB][rowIndex];
        
        else return m_rows[rowA][rowIndex] >= m_rows[rowB][rowIndex];
    }
    
    // Ascending
    if (sortCriteria[sortIndex].ordering == ot_ascending) {
        if (m_rows[rowA][rowIndex] < m_rows[rowB][rowIndex])
            return true;
        if (m_rows[rowA][rowIndex] > m_rows[rowB][rowIndex])
            return false;
    }
    
    // Descending
    else if (sortCriteria[sortIndex].ordering == ot_descending) {
        if (m_rows[rowA][rowIndex] > m_rows[rowB][rowIndex])
            return true;
        if (m_rows[rowA][rowIndex] < m_rows[rowB][rowIndex])
            return false;
    }
    
    // a = b, so check next criteria
    return comesFirst(rowA, rowB, sortCriteria, rowIndex, sortIndex+1);
}

int Database::split(vector<int> &a, int first, int last, const vector<SortCriterion> &sortCriteria)
{
    int i = first;
    int splitter = a[first]; // Pivot value
    do
    {
        // Find first value greater than or equal to  pivot
        while (first <= last && comesFirst(a[first], splitter, sortCriteria))
            first++;
        
        // Find first value less than pivot
        while (!comesFirst(a[last], splitter, sortCriteria))
            last--;
        
        // Move larger values to right, smaller values to left
        if (first < last)
            swap(a[first], a[last]);
    } while (first < last);
    
    // Place pivot in correct index
    swap(a[i], a[last]);
    i = last;
    return i;
}

void Database::sort(vector<int> &res, int start, int end, const vector<SortCriterion> &sortCriteria)
{
    if (end - start >= 1)
    {
        int splitter = split(res, start, end, sortCriteria); // Split
        sort(res, start, splitter-1, sortCriteria); // sort left side
        sort(res, splitter+1, end, sortCriteria);  // sort right side
    }
}

void Database::clearDatabase()
{
    for (int i = 0; i < m_field.size(); i++) {
        delete m_field[i]; // Delete all dynamically allocated fields
    }
    
    // Clear all rows, fields, and schema
    m_rows.clear();
    m_field.clear();
    m_schema.clear();
}

