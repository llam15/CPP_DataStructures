#include <string>
//#include <iostream>
#include <stack>

using namespace std;


// Coordinate Class
class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};



bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec){
    // Return true if there is a path from (sr,sc) to (er,ec)
    // through the maze; return false otherwise
    
    stack<Coord> coordStack;
    Coord start(sr,sc);
    
    coordStack.push(start); // Push starting coordinate
    maze[sr][sc] = '*'; // Coordinate encountered
    
    while (!coordStack.empty()) { // While stack not empty
        Coord current = coordStack.top(); // Pop top coordinate for current
        coordStack.pop();
        
        if (current.r() == er && current.c() == ec) { // If current = end, then solved
            return true;
        }
        
        Coord NORTH(current.r()-1,current.c());
        Coord EAST(current.r(), current.c()+1);
        Coord SOUTH(current.r()+1, current.c());
        Coord WEST(current.r(), current.c()-1);
        Coord dir[4] = {NORTH, EAST, SOUTH, WEST};
        
        for (int i = 0; i < 4; i++) { // For all 4 directions, check for valid coordinates
            if (dir[i].r() > 0 && dir[i].r() < nRows-1 && dir[i].c() > 0 && dir[i].c() < nCols-1
                && maze[dir[i].r()][dir[i].c()] != 'X' && maze[dir[i].r()][dir[i].c()] != '*') {
                
                coordStack.push(dir[i]);
                maze[dir[i].r()][dir[i].c()] = '*';
            }
        }
    }
    
    
    return false; // No solution
}

//int main()
//{
//    string maze[10] = {
//        "XXXXXXXXXX",
//        "X........X",
//        "XX.X.XXXXX",
//        "X..X.X...X",
//        "X..X...X.X",
//        "XXXX.XXX.X",
//        "X..X...XXX",
//        "X..XX.XX.X",
//        "X...X....X",
//        "XXXXXXXXXX"
//    };
//    
//    if (pathExists(maze, 10,10, 6,4, 1,1))
//        cout << "Solvable!" << endl;
//    else
//        cout << "Out of luck!" << endl;
//}

