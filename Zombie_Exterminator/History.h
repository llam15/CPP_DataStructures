
#ifndef __OrgZombieExterminator__History__
#define __OrgZombieExterminator__History__

#include "globals.h"

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
    
private:
    int m_rows, m_cols;
    int grid[MAXROWS][MAXCOLS];
};


#endif /* defined(__OrgZombieExterminator__History__) */
