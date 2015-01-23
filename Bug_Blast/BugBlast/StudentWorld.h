#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <vector>
#include <string>
class Level;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:

    StudentWorld();
    
    ~StudentWorld();
    
	virtual int init();

	virtual int move();

	virtual void cleanUp();
    
    void addActor(Actor *act);
    
    Actor* get(int x, int y) const;
    
    int getNumSprays() const { return m_numSprays; }
    
    void incrNumSprays() { m_numSprays++; }
    
    void decrNumSprays() { m_numSprays--; }
    
    void killZumi() { m_numZumi--; }
    
    Player* getPlayer() const { return m_player; }
    
    Level* currLevel() const { return m_lev; }
    
private:
    std::vector<Actor*> m_actors;
    Player *m_player;
    Level *m_lev;
    int m_numSprays;
    int m_numZumi;
    int m_levBonus;
    bool m_exitActive;
    
    void updateDisplayText();
    void decrBonus();
    void removeDeadActors();



    
};

#endif // STUDENTWORLD_H_
