#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


class Actor : public GraphObject
{
public:
    Actor(StudentWorld *m, int imageID, int startX, int startY)
    : GraphObject(imageID, startX, startY)
    {
        GraphObject::setVisible(true);
        m_alive = true;
        m_maze = m;
    }
    
    virtual ~Actor() {}
    
    virtual int moveMe() = 0;
    
    virtual bool canMove(int x, int y) const { return false; }
    
    bool isAlive() const { return m_alive; }
    
    void setAlive(bool alive) { m_alive = alive; }
    
    StudentWorld* getMaze() const { return m_maze; }
    
    bool sameSquareAsPlayer() const;
    
private:
    bool m_alive;
    StudentWorld *m_maze;
};

class Brick : public Actor
{
public:
    Brick(StudentWorld *m, int imageID, int startX, int startY)
    : Actor(m, imageID, startX, startY)
    {}
    
    virtual ~Brick() {}
    
    virtual int moveMe() { return -1; }

};

class permaBrick : public Brick
{
public:
    permaBrick(StudentWorld *m, int startX, int startY)
    : Brick(m, IID_PERMA_BRICK, startX, startY)
    {}
};

class destroyBrick : public Brick
{
public:
    destroyBrick(StudentWorld *m,int startX, int startY)
    : Brick(m, IID_DESTROYABLE_BRICK, startX, startY)
    {}
};

class Player : public Actor
{
public:
    Player(StudentWorld *m, int startX, int startY)
    : Actor(m, IID_PLAYER, startX, startY)
    {
        m_numSprays = 2;
        canWalkThruBricks = false;
        m_walkThruTicks = 0;
        m_extraSprayTicks = 0;
    }
    
    virtual int moveMe();
    
    void incrSpray(int num) { m_numSprays= num; }
    
    virtual bool canMove(int x, int y) const;
    
    void setWalkThruTicks (int t)
    {
        m_walkThruTicks = t;
        canWalkThruBricks = true;
    }
    
    void setExtraSprayTicks (int t) { m_extraSprayTicks = t; }
    
private:
    int m_numSprays;
    bool canWalkThruBricks;
    int m_walkThruTicks;
    int m_extraSprayTicks;
    
};

class Exit : public Actor
{
public:
    Exit(StudentWorld *m, int startX, int startY)
    : Actor(m, IID_EXIT, startX, startY)
    {
        setVisible(false);
        m_active = false;
    }
    
    virtual int moveMe();
    
    void activate();
    
private:
    bool m_active;
};

class Spray : public Actor
{
public:
    Spray(StudentWorld *m, int imageID, int startX, int startY)
    : Actor(m, imageID, startX, startY)
    {}
    
    virtual ~Spray() {}
    
    virtual int moveMe();
    
    virtual void explode() = 0;

    void setTicks(int i) { m_numticks = i; }
    
private:
    int m_numticks;
    
    
};

class bugSprayer : public Spray
{
public:
    bugSprayer(StudentWorld *m, int startX, int startY)
    : Spray(m, IID_BUGSPRAYER, startX, startY)
    {
        setTicks(40);
    }
    
    virtual void explode();
    
};

class bugSpray : public Spray
{
public:
    bugSpray(StudentWorld *m, int startX, int startY)
    : Spray(m, IID_BUGSPRAY, startX, startY)
    {
        setTicks(3);
    }
    
    virtual int moveMe();
    
    virtual void explode();
    
};

class simpleZumi : public Actor
{
public:
    //NORTH = 0
    //EAST = 1
    //SOUTH = 2
    //WEST = 3
    
    simpleZumi(StudentWorld *m, int startX, int startY, int moveTicks, int imageID = IID_SIMPLE_ZUMI)
    : Actor(m, imageID, startX, startY)
    {
        m_moveSpeed = moveTicks;
        m_currentDirection = randInt(0, 3);
        m_move = 0;
    }
    
    virtual ~simpleZumi() {}
    
    virtual int moveMe();
    
    virtual int makeMove();
    
    virtual bool canMove(int x, int y) const;
    
    void changeDirection(int dir = -1);
    
    virtual void kill();
    
    
private:
    int m_currentDirection;
    int m_moveSpeed;
    int m_move;
    
    void dropGoodie();
    int randInt(int lowest, int highest);


    
};

class complexZumi : public simpleZumi
{
public:
    complexZumi(StudentWorld *m,int startX, int startY, int moveTicks, int smell)
    : simpleZumi(m, startX, startY, moveTicks, IID_COMPLEX_ZUMI)
    {
        m_smellDistance = smell;
    }
    
    struct Coord
    {
        Coord(int x, int y) {
            m_x = x;
            m_y = y;
        }
        int m_x;
        int m_y;
        int first = -1;
    };
    
    virtual int makeMove();
    virtual void kill();
    
private:
    int m_smellDistance;
    
    int findPath(int sx, int sy, int ex, int ey);

};

class Goodie : public Actor
{
public:
    Goodie(StudentWorld *m, int imageID, int startX, int startY, int lifetime)
    : Actor(m, imageID, startX, startY)
    {
        m_lifetime = lifetime;
    }
    
    virtual ~Goodie() {}
    
    virtual int moveMe();
    
    virtual void award();
        
    
private:
    int m_lifetime;
    
};

class extraLifeGoodie : public Goodie
{
public:
    extraLifeGoodie(StudentWorld *m,int startX, int startY, int lifetime)
    : Goodie(m, IID_EXTRA_LIFE_GOODIE, startX, startY, lifetime)
    {}
    
    virtual void award();

};

class walkThruWallsGoodie : public Goodie
{
public:
    walkThruWallsGoodie(StudentWorld *m,int startX, int startY, int lifetime)
    : Goodie(m, IID_WALK_THRU_GOODIE, startX, startY, lifetime)
    {}
    
    virtual void award();
    
};

class increaseSprayersGoodie : public Goodie
{
public:
    increaseSprayersGoodie(StudentWorld *m,int startX, int startY, int lifetime)
    : Goodie(m, IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE, startX, startY, lifetime)
    {}
    
    virtual void award();
    
};




#endif // ACTOR_H_
