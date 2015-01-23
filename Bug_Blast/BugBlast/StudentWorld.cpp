#include "StudentWorld.h"
#include "Level.h"
#include "GameConstants.h"
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;

GameWorld* createStudentWorld()
{
	return new StudentWorld();
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld()
{
    
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    m_numZumi = 0;
    m_numSprays = 0;
    m_exitActive = false;
    
    ostringstream oss;
    int numLevel = getLevel();
    oss.fill('0');
    oss << setw(2) << numLevel;
//    Level lev;
    m_lev = new Level;
	string current = "level" + oss.str() + ".dat";
    Level::LoadResult result = m_lev->loadLevel(current);
    if (result == Level::load_fail_file_not_found && getLevel() == 0)
        return GWSTATUS_NO_FIRST_LEVEL;
    if (result == Level::load_fail_file_not_found && getLevel() != 0)
        return GWSTATUS_PLAYER_WON;
    if (result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    
    if (result == Level::load_success)
    {
        m_levBonus = m_lev->getOptionValue(optionLevelBonus);
        
        Level::MazeEntry obj;
        for (int x = 0; x < VIEW_WIDTH; x++) {
            for (int y = 0; y < VIEW_HEIGHT; y++) {
                obj = m_lev->getContentsOf(x, y);
                switch (obj) {
                    case Level::player:
                        m_player = new Player(this,x,y);
                        break;
                        
                    case Level::destroyable_brick:
                        m_actors.push_back(new destroyBrick(this,x,y));
                        break;
                        
                    case Level::perma_brick:
                        m_actors.push_back(new permaBrick(this,x,y));
                        break;
                        
                    case Level::complex_zumi:
                        m_actors.push_back(new complexZumi(this,x,y,m_lev->getOptionValue(optionTicksPerComplexZumiMove), m_lev->getOptionValue(optionComplexZumiSearchDistance)));
                        m_numZumi++;
                        break;
                        
                    case Level::simple_zumi:
                        m_actors.push_back(new simpleZumi(this,x,y, m_lev->getOptionValue(optionTicksPerSimpleZumiMove)));
                        m_numZumi++;
                        break;
                        
                    case Level::exit:
                        m_actors.push_back(new Exit(this,x,y));
                        break;
                      
                    default:
                        break;
                }
            }
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::removeDeadActors()
{
    for (int i = 0; i < m_actors.size(); i++) {
        Actor* a = m_actors[i];
        if (!a->isAlive()) {
            delete a;
            m_actors[i] = m_actors[m_actors.size()-1];
            m_actors.pop_back();
            i--;
        }
    }
    
}

void StudentWorld::updateDisplayText()
{
    
    
    int score = getScore();
    int level = getLevel();
    int livesLeft = getLives();
    
    ostringstream oscr;
    oscr.fill('0');
    oscr << setw(8) << score;
    
    ostringstream olev;
    olev.fill('0');
    olev << setw(2) << level;
    
    ostringstream olive;
    olive.fill('0');
    olive << setw(3) << livesLeft;
    
    ostringstream obon;
    obon << setw(6) << m_levBonus;
    
    string stat = "Score: " + oscr.str() + "  Level: " + olev.str() +
            "  Lives: " + olive.str() + "  Bonus: " + obon.str();
    
    setGameStatText(stat);
}

void StudentWorld::decrBonus()
{
    m_levBonus--;
    if (m_levBonus <= 0) {
        m_levBonus = 0;
    }
}

//INCREASE SCORE,
int StudentWorld::move()
{
    // UPDATE DISPLAY TEXT
    updateDisplayText();
    
    //Move actors
    int playerMove = m_player->moveMe();
    for (int i = 0; i < m_actors.size(); i++) {
        Actor* a = m_actors[i];
        if (a->isAlive()) {
            int actorMove = a->moveMe();
            
            if (!m_player->isAlive()) {
                decLives();
                playSound(SOUND_PLAYER_DIE);
                return GWSTATUS_PLAYER_DIED;
            }
            
            if (playerMove == GWSTATUS_FINISHED_LEVEL || actorMove == GWSTATUS_FINISHED_LEVEL) {
                increaseScore(m_levBonus);
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
    }
    
    //REMOVE DEAD
    removeDeadActors();
    
    //DECREASE BONUS
    decrBonus();
    
    if (m_numZumi == 0 && !m_exitActive) {
        Exit* e;
        for (int i = 0; i < m_actors.size(); i++) {
            e = dynamic_cast<Exit*>(m_actors[i]);
            if (e != nullptr) {
                e->activate();
            }
        }
        m_exitActive = true;
    }
    
    return playerMove;
}

void StudentWorld::cleanUp()
{
    delete m_player;
    
    for (int i = 0; i < m_actors.size(); i++) {
        delete m_actors[i];
    }
    m_actors.clear();
    
    delete m_lev;
}

void StudentWorld::addActor(Actor *act)
{
    m_actors.push_back(act);
}

Actor* StudentWorld::get(int x, int y) const
{
    
    Actor* temp;
    for (int i = 0; i < m_actors.size(); i++) {
        temp = m_actors[i];
        if (temp->getX() == x && temp->getY() == y) {
            return temp;
        }
    }
    
    
    
    return nullptr;
}