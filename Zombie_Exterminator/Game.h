
#ifndef __OrgZombieExterminator__Game__
#define __OrgZombieExterminator__Game__

#include <string>

class Arena;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nZombies);
    ~Game();
    
    // Mutators
    void play();
    bool recommendMove(const Arena& a, int r, int c, int& bestDir);
    int computeDanger(const Arena& a, int r, int c);
    
private:
    Arena* m_arena;
    
    // Helper functions
    std::string takePlayerTurn();
};

#endif /* defined(__OrgZombieExterminator__Game__) */
