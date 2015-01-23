#include "Actor.h"
#include "StudentWorld.h"
#include "Level.h"
#include "GameConstants.h"
#include <algorithm>
#include <queue>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

bool Actor::sameSquareAsPlayer() const
{
    Player *p = getMaze()->getPlayer();
    if (p->getX() == getX() && p->getY() == getY()) {
        return true;
    }
    
    return false;
}

bool Player::canMove(int x, int y) const
{
    Actor* next = getMaze()->get(x, y);
    if (canWalkThruBricks) {
        if (dynamic_cast<destroyBrick*>(next) != nullptr) {
            return true;
        }
    }
    
    if (dynamic_cast<Brick*>(next) != nullptr) {
        return false;
    }
    
    return true;
}

int Player::moveMe()
{
    if (!isAlive()){
        return GWSTATUS_PLAYER_DIED;
    }
    
    Actor* curr = getMaze()->get(getX(), getY());
    
    if (dynamic_cast<simpleZumi*>(curr) != nullptr) {
        getMaze()->playSound(SOUND_PLAYER_DIE);
        return GWSTATUS_PLAYER_DIED;
    }
    
    if (dynamic_cast<Brick*>(curr) != nullptr && !canWalkThruBricks) {
        getMaze()->playSound(SOUND_PLAYER_DIE);
        return GWSTATUS_PLAYER_DIED;
    }
    
    if (canWalkThruBricks) {
        m_walkThruTicks--;
        if (m_walkThruTicks <= 0) {
            canWalkThruBricks = false;
        }
    }
    
    if (m_numSprays > 2) {
        m_extraSprayTicks--;
        if (m_extraSprayTicks <= 0) {
            m_numSprays = 2;
        }
    }
    
    
    int key;
    if (getMaze()->getKey(key)) {
        switch (key)
        {
            case KEY_PRESS_LEFT:
                if (canMove(getX()-1, getY()))
                    moveTo(getX()-1, getY());
                break;
            case KEY_PRESS_RIGHT:
                if (canMove(getX()+1, getY()))
                    moveTo(getX()+1, getY());
                break;
            case KEY_PRESS_UP:
                if (canMove(getX(), getY()+1))
                    moveTo(getX(), getY()+1);
                break;
            case KEY_PRESS_DOWN:
                if (canMove(getX(), getY()-1))
                    moveTo(getX(), getY()-1);
                break;

            case KEY_PRESS_SPACE:
                if (getMaze()->getNumSprays() < m_numSprays &&
                        dynamic_cast<Brick*>(curr) == nullptr &&
                        dynamic_cast<bugSprayer*>(curr) == nullptr) {
                    getMaze()->addActor(new bugSprayer(getMaze(), getX(), getY()));
                    getMaze()->incrNumSprays();
                }
                break;
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;

}

void Exit::activate()
{
    m_active = true;
    setVisible(true);
    getMaze()->playSound(SOUND_REVEAL_EXIT);
}

int Exit::moveMe()
{
    if (m_active) {
        if (sameSquareAsPlayer()) {
            getMaze()->playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    
    return -1;
}

int Spray::moveMe()
{
    
    if (!isAlive()) {
        return -1;
    }
    
    if (m_numticks <= 0) {
        explode();
    }
    
    else m_numticks--;
    
    return GWSTATUS_CONTINUE_GAME;
}

void bugSprayer::explode()
{
    getMaze()->decrNumSprays();
    
    Actor* next;
    for (int i = 0; i <=2; i++) {
        next = getMaze()->get(getX()+i, getY());
            if (dynamic_cast<permaBrick*>(next) == nullptr) {
                getMaze()->addActor(new bugSpray(getMaze(), getX()+i, getY()));
            }
        
            if (dynamic_cast<Brick*>(next) != nullptr) {
                break;
            }
        
    }
    
    for (int i = 1; i <=2; i++) {
        next = getMaze()->get(getX()-i, getY());
        if (dynamic_cast<permaBrick*>(next) == nullptr) {
            getMaze()->addActor(new bugSpray(getMaze(), getX()-i, getY()));
        }
        
        if (dynamic_cast<Brick*>(next) != nullptr) {
            break;
        }
    }
    
    for (int i = 1; i <=2; i++) {
        next = getMaze()->get(getX(), getY()+i);
        if (dynamic_cast<permaBrick*>(next) == nullptr) {
            getMaze()->addActor(new bugSpray(getMaze(), getX(), getY()+i));
        }
        
        if (dynamic_cast<Brick*>(next) != nullptr) {
            break;
        }
        
    }
    
    for (int i = 1; i <=2; i++) {
        next = getMaze()->get(getX(), getY()-i);
        if (dynamic_cast<permaBrick*>(next) == nullptr) {
            getMaze()->addActor(new bugSpray(getMaze(), getX(), getY()-i));
        }
        
        if (dynamic_cast<Brick*>(next) != nullptr) {
            break;
        }
        
    }
    
    getMaze()->playSound(SOUND_SPRAY);
    setAlive(false);
}

int bugSpray::moveMe()
{
    Spray::moveMe();
    
    if (isAlive()) {
        Actor* curr = getMaze()->get(getX(), getY());
        if (curr != nullptr) {
            
            if (sameSquareAsPlayer()) {
                getMaze()->getPlayer()->setAlive(false);
                return GWSTATUS_PLAYER_DIED;
            }
            
            
            if (dynamic_cast<simpleZumi*>(curr) != nullptr){
                simpleZumi* z = dynamic_cast<simpleZumi*>(curr);
                z->kill();
                getMaze()->killZumi();
            }
            
            else if (dynamic_cast<destroyBrick*>(curr) != nullptr) {
                curr -> setAlive(false);
            }
            
            else if (dynamic_cast<bugSprayer*>(curr) != nullptr) {
                bugSprayer* b = dynamic_cast<bugSprayer*>(curr);
                b->setTicks(0);
            }
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void bugSpray::explode()
{
    setAlive(false);
}

int simpleZumi::randInt(int lowest, int highest)
{
    if (highest < lowest)
        std::swap(highest, lowest);
    return lowest + (rand() % (highest - lowest + 1));
}

void simpleZumi::changeDirection(int i)
{
    if (i != -1) {
        m_currentDirection = i;
        return;
    }
    
    int oldDir = m_currentDirection;
    while (oldDir == m_currentDirection) {
        m_currentDirection = randInt(0, 3);
    }
}

bool simpleZumi::canMove(int x, int y) const
{
    Actor* next = getMaze()->get(x, y);
    if (dynamic_cast<Brick*>(next) != nullptr || dynamic_cast<bugSprayer*>(next) != nullptr) {
        return false;
    }
    
    return true;
}

int simpleZumi::makeMove()
{
    int nextX = getX();
    int nextY = getY();
    switch (m_currentDirection) {
        case 0:
            nextY++;
            break;
        case 1:
            nextX++;
            break;
        case 2:
            nextY--;
            break;
        case 3:
            nextX--;
            break;
            
        default:
            break;
    }
    
    if (canMove(nextX, nextY)) {
        moveTo(nextX, nextY);
    }
    
    else changeDirection();
    
    return GWSTATUS_CONTINUE_GAME;
}

int complexZumi::findPath(int sx, int sy, int ex, int ey)
{
    std::queue<Coord> coordQueue;
    Coord start(sx,sy);
    
    char visited[VIEW_WIDTH][VIEW_HEIGHT];
    for (int x = 0; x < VIEW_WIDTH; x++) {
        for (int y = 0; y < VIEW_HEIGHT; y++) {
            visited[x][y] = '.';
        }
    }
    
    coordQueue.push(start); // Push starting coordinate
    visited[sx][sy] = '*'; // Coordinate encountered
    
    while (!coordQueue.empty()) { // While stack not empty
        Coord current = coordQueue.front(); // Pop top coordinate for current
        coordQueue.pop();
        
        if (current.m_x == ex && current.m_y == ey) { // If current = end, then solved
            
            return current.first;
        }

        Coord n(current.m_x, current.m_y+1);
        Coord e(current.m_x+1, current.m_y);
        Coord s(current.m_x, current.m_y-1);
        Coord w(current.m_x-1, current.m_y);

        Coord dir[4] = {n, e, s, w};
        

        for (int i = 0; i < 4; i++) { // For all 4 directions, check for valid coordinates
            if (canMove(dir[i].m_x, dir[i].m_y) && visited[dir[i].m_x][dir[i].m_y] != '*') {
                
                
                if (current.first == -1) {
                    dir[i].first = i;
                }
                
                else dir[i].first = current.first;
                
                visited[dir[i].m_x][dir[i].m_y] = '*';
                coordQueue.push(dir[i]);
            }
        }
    }
    
    return -1;
}

int complexZumi::makeMove()
{
    int horizDist = abs(getMaze()->getPlayer()->getX() - getX());
    int vertDist = abs(getMaze()->getPlayer()->getY() - getY());
    

    if (horizDist <= m_smellDistance && vertDist <= m_smellDistance) {
        //FIND BEST PATH
        int next = findPath(getX(), getY(), getMaze()->getPlayer()->getX(), getMaze()->getPlayer()->getY());
        if (next != -1) {
            changeDirection(next);
        }
    }
    
    simpleZumi::makeMove();
    
    return GWSTATUS_CONTINUE_GAME;
}

int simpleZumi::moveMe()
{
    
    if (!isAlive()) {
        return -1;
    }
    
    if (sameSquareAsPlayer()) {
        getMaze()->getPlayer()->setAlive(false);
        return GWSTATUS_PLAYER_DIED;
    }
    
    if (m_move == m_moveSpeed) {
        m_move = 0;
        
        return makeMove();
    }
    
    m_move++;
    
    return GWSTATUS_CONTINUE_GAME;
}

void simpleZumi::kill()
{
    setAlive(false);
    getMaze()->playSound(SOUND_ENEMY_DIE);
    getMaze()->increaseScore(100);
    dropGoodie();
}

void complexZumi::kill()
{
    simpleZumi::kill();
    getMaze()->increaseScore(400);
}

void simpleZumi::dropGoodie()
{
    int goodieProb = getMaze()->currLevel()->getOptionValue(optionProbOfGoodieOverall);
    if (randInt(0, 99) < goodieProb) {
        int extraLifeProb = getMaze()->currLevel()->getOptionValue(optionProbOfExtraLifeGoodie);
        int thruWallsProb = getMaze()->currLevel()->getOptionValue(optionProbOfWalkThruGoodie);
        int sprayerProb = getMaze()->currLevel()->getOptionValue(optionProbOfMoreSprayersGoodie);
        
        int sum[3] = {extraLifeProb, extraLifeProb+thruWallsProb, extraLifeProb+thruWallsProb+sprayerProb};
        
        int choose = randInt(1, sum[2]);
        
        if (choose <= sum[0]) {
            getMaze()->addActor(new extraLifeGoodie(getMaze(), getX(), getY(), getMaze()->currLevel()->getOptionValue(optionGoodieLifetimeInTicks)));
        }
        else if (choose <= sum[1]) {
            getMaze()->addActor(new walkThruWallsGoodie(getMaze(), getX(), getY(), getMaze()->currLevel()->getOptionValue(optionGoodieLifetimeInTicks)));
        }
        else if (choose <= sum[2]) {
            getMaze()->addActor(new increaseSprayersGoodie(getMaze(), getX(), getY(), getMaze()->currLevel()->getOptionValue(optionGoodieLifetimeInTicks)));
        }
    }
}

void Goodie::award()
{
    getMaze()->increaseScore(1000);
}

int Goodie::moveMe()
{
    if (!isAlive()) {
        return -1;
    }
    
    m_lifetime--;
    
    if (m_lifetime <= 0) {
        setAlive(false);
    }
    
    if (sameSquareAsPlayer()) {
        award();
        setAlive(false);
        getMaze()->playSound(SOUND_GOT_GOODIE);
    }
    
    return -1;
}

void extraLifeGoodie::award()
{
    Goodie::award();
    getMaze()->incLives();
}

void walkThruWallsGoodie::award()
{
    Goodie::award();
    getMaze()->getPlayer()->setWalkThruTicks(getMaze()->currLevel()->getOptionValue(optionWalkThruLifetimeTicks));
}

void increaseSprayersGoodie::award()
{
    Goodie::award();
    getMaze()->getPlayer()->setExtraSprayTicks(getMaze()->currLevel()->getOptionValue(optionBoostedSprayerLifetimeTicks));
    
    int maxSprays = getMaze()->currLevel()->getOptionValue(optionMaxBoostedSprayers);
    getMaze()->getPlayer()->incrSpray(maxSprays);
    
}


