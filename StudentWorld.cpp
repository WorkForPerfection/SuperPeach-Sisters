#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"

#include <string>
#include <iostream>
#include <sstream>
#define StudentWorld sw
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new sw(assetPath);
}

// Students:  Add code to this file, sw.h, Actor.h, and Actor.cpp

sw::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int sw::init()
{
    Level thislevel(assetPath());
    ostringstream oss;
    oss.setf(ios::fixed);
    oss.precision(2);
    oss << "level0" << getLevel()  << ".txt";
    string it = oss.str();
    Level :: LoadResult result = thislevel.loadLevel(it);
    if (result == Level::load_fail_file_not_found)
        cerr << "Could not find level01.txt data file" << endl;
    else if (result == Level::load_fail_bad_format)
        cerr << "level01.txt is improperly formatted" << endl;
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
        for(int x=0; x<GRID_HEIGHT; ++x)
        {
            for(int y=0; y<GRID_WIDTH; ++y)
            {
                Level::GridEntry ge;
                ge = thislevel.getContentsOf(x, y); // x=5, y=10
                switch (ge){ 
                    case Level::empty:
                        break; 
                    case Level::peach:
                        mpeach = new Peach(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH);
                        break;
                    case Level::mario:
                        actors.push_back(new Mario(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH));
                        break;
                    case Level::piranha:
                        actors.push_back(new Piranha(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH));
                        break;
                    case Level::koopa:
                        actors.push_back(new Koopa(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH));
                        break;
                    case Level::goomba:
                        actors.push_back(new Goomba(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH));
                        break;
                    case Level::flag:
                        actors.push_back(new Flags(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH));
                        break;
                    case Level::pipe:
                        actors.push_back(new Pipe(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH));
                        break;
                    case Level::block:
                        actors.push_back(new Block(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH, 0));
                        break;
                    case Level::flower_goodie_block:
                        actors.push_back(new Block(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH, 1));
                        break;
                    case Level::mushroom_goodie_block:
                        actors.push_back(new Block(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH, 2));
                        break;
                    case Level::star_goodie_block:
                        actors.push_back(new Block(this, x*SPRITE_HEIGHT, y*SPRITE_WIDTH, 3));
                        break;;
                }
            }
        }
        
    }
    return GWSTATUS_CONTINUE_GAME;
}

int sw::move(){
    ostringstream oss;
    oss << " Lives: " << getLives();
    oss << " Level: 0" << getLevel();
    int score = getScore();
    string to = "00000";
    if(score>=1000) to = "00";
    else if(score>=100) to = "000";
    else if(score>=10) to = "0000";
    oss << " Points: "<< to << getScore();
    if(mpeach->hasinv()) oss<<"StarPower!";
    if(mpeach->hasshoot()) oss<<" ShootPower!";
    if(mpeach->hasjump()) oss<<" JumpPower!";
    setGameStatText(oss.str());
    mpeach->doSomething();
    for(int i=0; i<actors.size(); ++i)  actors[i]->doSomething();
    if(!mpeach->isAlive()) {playSound(SOUND_PLAYER_DIE);decLives();return GWSTATUS_PLAYER_DIED;}
    if(gameover == true){gameover = false;playSound(SOUND_GAME_OVER); return GWSTATUS_PLAYER_WON;}
    if(level_finished == true){level_finished = false;playSound(SOUND_FINISHED_LEVEL); return GWSTATUS_FINISHED_LEVEL;}
    vector<Actor*>::iterator it = actors.begin();
    while (it != actors.end()) {
        if ((*it)->isAlive() == false) {
            delete (*it);
            it = actors.erase(it);
        }
        else it++;
    }
    return GWSTATUS_CONTINUE_GAME;
}

void sw::cleanUp()
{
    vector<Actor*>::iterator it = actors.begin();
    while (it != actors.end()) {
        delete (*it);
        it = actors.erase(it);
    }
    delete mpeach;
}

void sw :: bonkat(int x, int y)
{
    for(int i=0; i< actors.size(); ++i){
        if(actors[i]->isblock() && actors[i]->isAlive()){
            double dx = x - actors[i]->getX();  double dy = y - actors[i]->getY();
            if(-(SPRITE_WIDTH - 1) <= dx && dx <= SPRITE_WIDTH - 1 && -(SPRITE_HEIGHT - 1) <= dy && dy <= SPRITE_HEIGHT - 1){
                actors[i]->bonked();
            }
        }
    }
}

bool sw :: hasblock(int x, int y){
    vector<Actor*>::iterator it = actors.begin();
    while (it != actors.end()){
        if((*it)->isblock()){
            double dx = x - (*it)->getX();double dy = y - (*it)->getY();
            if(-(SPRITE_WIDTH - 1) <= dx && dx <= SPRITE_WIDTH - 1 && -(SPRITE_HEIGHT - 1) <= dy && dy <= SPRITE_HEIGHT - 1){
                return true;
            }
        }
        it++;
    }
    return false;
}

bool sw :: damage(int x, int y){
    for(int i=0; i< actors.size(); ++i){
        if(actors[i]->isEnemy() && actors[i]->isAlive()){
            double dx = x - actors[i]->getX(); double dy = y - actors[i]->getY();
            if(-(SPRITE_WIDTH - 1) <= dx && dx <= SPRITE_WIDTH - 1 && -(SPRITE_HEIGHT - 1) <= dy && dy <= SPRITE_HEIGHT - 1)
                return true;
        }
    }
    return false;
}

void sw :: damageat(int x, int y){
    for(int i=0; i< actors.size(); ++i){
        if(actors[i]->isEnemy() && actors[i]->isAlive()){            
            double dx = x - actors[i]->getX(); double dy = y - actors[i]->getY();
            if(-(SPRITE_WIDTH - 1) <= dx && dx <= SPRITE_WIDTH - 1 && -(SPRITE_HEIGHT - 1) <= dy && dy <= SPRITE_HEIGHT - 1){
                actors[i]->kill();
                if(actors[i]->isG() || actors[i]->isP()) increaseScore(100); //for goomba and piranhas
                else if(actors[i]->isK()){
                    addactor(new Shell(this, actors[i]->getX(), actors[i]->getY(), actors[i]->getDirection()));
                    increaseScore(100);
                }
            }
        }
    }
}

void sw :: addactor(Actor* actor){
    actors.push_back(actor);
}

//will only be required to set to 2
void sw::setpeachhp()
{
    mpeach->sethitpoint();
}


bool sw :: overlap_peach(int x, int y){
    double dx = x - mpeach->getX(), dy = y - mpeach->getY();
    if(-(SPRITE_WIDTH - 1) <= dx && dx <= SPRITE_WIDTH - 1 && -(SPRITE_HEIGHT - 1) <= dy && dy <= SPRITE_HEIGHT - 1)
        return true;
    return false;
}
