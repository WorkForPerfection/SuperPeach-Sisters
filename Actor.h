#ifndef ACTOR_H_
#define ACTOR_H_
#define StudentWorld sw
#include "GraphObject.h"

class sw;

// Students:  Add code to this file, Actor.cpp, sw.h, and sw.cpp
class Actor : public GraphObject
{
public:
    Actor(sw* world, int imageID, int startX, int startY, int dir, int depth, double size);
    bool isAlive() const;
    virtual bool isblock(){return false;}
    //checks if it is enemy
    virtual bool isEnemy(){return false;}
    virtual bool isG(){return false;}
    virtual bool isK(){return false;}
    virtual bool isP(){return false;}
    //kill actor
    void kill();
    virtual void bonked(){};
    virtual void doSomething()=0;
    sw* getWorld();
    virtual ~Actor();
private:
    bool alive;
    sw* w;
};


class Peach : public Actor
{
public:
    Peach(sw* w, int startX, int startY);
    void doSomething();
    void bonked();
    //simple utility functions checking outcomes of an action
    bool hasinv(){return invincible!=0;}
    bool hasjump(){return jump_power!=0;}
    bool hasshoot(){return shoot_power!=0;}
    void sethitpoint(){hitpoint=2;}
    void get_shoot(){shoot_power = 1;}
    void get_jump(){jump_power = 1;}
    void get_star(){invincible = 150;}
    virtual ~Peach();
private:
    int invincible;
    int time_to_recharge_before_next_fire;
    int hitpoint;
    int jump_power;
    int shoot_power;
    int jumpdistance;
    
};

class Block : public Actor
{
public:
    Block(sw* w, int startX, int startY, int ggoodie);
    void doSomething();
    bool isblock(){return true;}
    void bonked();
    virtual ~Block();
private:
    int type_of_goodie;
};

class Pipe : public Actor
{
public:
    Pipe(sw* w, int startX, int startY);
    void doSomething();
    bool isblock(){return true;}
    void bonked();
    virtual ~Pipe();
};

class Flags : public Actor
{
public:
    Flags(sw* w, int startX, int startY);
    void doSomething();
    void bonked(){}
    virtual ~Flags();
};

class Mario : public Actor
{
public:
    Mario(sw* w, int startX, int startY);
    void doSomething();
    void bonked(){}
    virtual ~Mario();
};

class Flower : public Actor
{
public:
    Flower(sw* w, int startX, int startY);
    void doSomething();
    virtual ~Flower();
};

class Mushroom : public Actor
{
public:
    Mushroom(sw* w, int startX, int startY);
    void doSomething();
    virtual ~Mushroom();
};

class Star : public Actor
{
public:
    Star(sw* w, int startX, int startY);
    void doSomething();
    virtual ~Star();
};

class Goomba : public Actor
{
public:
    Goomba(sw* w, int startX, int startY);
    void doSomething();
    bool isEnemy(){return true;}
    void bonked();
    bool isG(){return true;}
    virtual ~Goomba(){}
};

class Koopa : public Actor
{
public:
    Koopa(sw* w, int startX, int startY);
    void doSomething();
    bool isEnemy(){return true;}
    void bonked();
    bool isK(){return true;}
    virtual ~Koopa(){}
};

class Piranha : public Actor
{
public:
    Piranha(sw* w, int startX, int startY);
    void doSomething();
    bool isEnemy(){return true;}
    void bonked();
    bool isP(){return true;}
    virtual ~Piranha(){}
private:
    int fire_delay=0;
};

class Piranha_fireball : public Actor
{
public:
    Piranha_fireball(sw* w, int startX, int startY, int direction);
    void doSomething();
    virtual ~Piranha_fireball(){}
private:
    int dir;
};

class Peach_fireball : public Actor
{
public:
    Peach_fireball(sw* w, int startX, int startY, int direction);
    void doSomething();
    virtual ~Peach_fireball(){}
private:
    int dir;
};

class Shell : public Actor
{
public:
    Shell(sw* w, int startX, int startY, int direction);
    void doSomething();
    virtual ~Shell(){}
private:
    int dir;
};

#endif // ACTOR_H_
