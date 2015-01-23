
#include <iostream>
#include "Map.h"
#include <cassert>
using namespace std;


int main()
{
    // default constructor
    Map m;
    
    // For an empty map:
    assert(m.size() == 0);      // test size
    assert(m.empty());          // test empty
    assert(!m.erase("Ricky"));  // nothing to erase
    
    //For non-empty map
    m.insert("Hello", 1000);
    assert(m.contains("Hello") && m.size() == 1);   // test size and if inserted properly
    assert(m.erase("Hello"));                       // test erase
    assert(m.size() == 0);                          // test if erased properly
    m.insert("A", 100);
    m.insert("B", 1030);
    m.insert("C", 10000);
    
    KeyType str;
    ValueType v;
    assert(m.get(0, str, v));                       // test 3-parameter get function
    assert(str == "C");
    assert(v = 10000);
    assert(m.insertOrUpdate("C", 1));               // test insert or update function (update)
    assert(m.get("C", v));                          // test 2-parameter get function
    assert(v == 1);
    assert(m.insertOrUpdate("X", 4000));            // test insert or update function (insert)
    assert(m.contains("X"));
    m.erase("X");
    
    Map m2;
    m2.insert("X", 1000);
    m2.insert("Y", 10300);
    m2.insert("Z", 101);
    m2.insert("Phi", 111);
    
    m2.swap(m);                                     // test swap function
    assert(m.size() == 4 && m.contains("X") && m.contains("Y") && m.contains("Z") && m.contains("Phi"));
    assert(m2.size() == 3 && m2.contains("A") && m2.contains("B") && m2.contains("C"));
    
    
    Map m3;
    m3 = m2;                                        // test assignment operator
    assert(m3.size() == 3 && m3.contains("A") && m3.contains("B") && m3.contains("C"));
    
    Map mm = Map(m2);                               // test copy constructor
    assert(mm.size() == 3 && mm.contains("A") && mm.contains("B") && mm.contains("C"));

    Map m4;
    m4.insert("L", 100);
    m4.insert("M", 100);
    m4.insert("N", 100);
    
    Map m5;
    m5.insert("L", 100);
    m5.insert("M", 9);
    m5.insert("O", 500);

    assert(!combine(m4, m5, m3));                   // Test combine with doubled key with different value
    assert(m3.size() == 3 && m3.contains("L") && m3.contains("N") && m3.contains("O"));

    
    m5.update("M", 100);
    assert(combine(m4, m5, m3));                    // Test combine with doubled keys, but same values
    assert(m3.size() == 4 && m3.contains("L") && m3.contains("M") && m3.contains("N") && m3.contains("O"));
    
    Map m6;
    assert(combine(m4, m6, m3));                    // Test combine with empty map
    assert(m3.size() == 3 && m3.contains("L") && m3.contains("M") && m3.contains("N"));
    
    assert(combine(m4, m2, m3));                    // Test combine with no doubled keys
    assert(m3.size() == 6 && m3.contains("L") && m3.contains("M") && m3.contains("N") && m3.contains("A")
           && m3.contains("B") && m3.contains("C"));
    
    subtract(m4, m5, m3);                           // Test subtract function
    assert(m3.size() == 1 && m3.contains("N"));
    
    subtract(m4, m5, m4);                           // Test subtract with aliasing possibility
    assert(m4.size() == 1 && m4.contains("N"));
    
    
    cout << "Passed all tests" << endl;
    
}

