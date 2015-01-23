#include "History.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols){
    m_rows = nRows;
    m_cols = nCols;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            grid[r-1][c-1] = 0;
            
}


bool History::record(int r, int c){
    
    if (r < 1 || c < 1 || r > m_rows || c > m_cols) {
        return false;
    }
    
    grid[r-1][c-1]++;
    
    return true;
}


void History::display() const{
    clearScreen();
    for (int r = 1; r <= m_rows; r++){
        for (int c = 1; c <= m_cols; c++){
            if (grid[r-1][c-1] == 0) {
                cout << ".";
            }
            else if (grid[r-1][c-1] < 26)
                cout << (char) ('A' + grid[r-1][c-1]-1);
            else cout << "Z";            
        }
        cout << endl;
    }
    cout << endl;
    
}