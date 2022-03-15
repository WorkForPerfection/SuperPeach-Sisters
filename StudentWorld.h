#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void bonkat(int x, int y);
    void damageat(int x, int y);
    bool hasblock(int x, int y);
    bool damage(int x, int y);
    void addactor(Actor* actor);
    bool overlap_peach(int x, int y);
    //peach
    void setpeachhp();
    Peach* getPeach(){return mpeach;}
    void get_shoot_power(){mpeach->get_shoot();}
    void get_jump_power(){mpeach->get_jump();}
    void get_star_power(){mpeach->get_star();}
    void bonkpeach(){mpeach->bonked();}
    //game
    void level_complete(){level_finished = true;}
    void finish_game(){gameover = true;}
private:
    vector<Actor*> actors;
    Peach* mpeach;
    Flags* myflag;
    bool level_finished=false;
    bool gameover=false;
};

#endif // STUDENTWORLD_H_
