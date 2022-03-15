#include "Actor.h"
#include "StudentWorld.h"
#define StudentWorld sw
using namespace std;

//actor utilities
//-------------------------------------------------------------------
Actor :: Actor(sw* world, int imageID, int startX, int startY, int dir, int depth, double size): GraphObject(imageID, startX, startY, dir, depth, size) {w = world;alive=true;}


bool Actor :: isAlive() const{
    return alive;
}

void Actor :: kill(){
    alive = false;
}

sw* Actor :: getWorld(){
    return w;
}

Actor :: ~Actor(){}

//peach class
//-------------------------------------------
Peach :: Peach(sw* world, int startX, int startY) : Actor(world, IID_PEACH, startX, startY, 0, 0, 1)
{
    invincible = 0;
    jump_power = 0;
    shoot_power = 0;
    hitpoint=1;
    jumpdistance = 0;
    time_to_recharge_before_next_fire=0;
}

void Peach :: bonked()
{
    if(invincible!=0)return;
    invincible = 10;
    hitpoint--;
    if(shoot_power!=0) shoot_power = 0;
    if(jump_power!=0) jump_power = 0;
    if(hitpoint>0)getWorld()->playSound(SOUND_PLAYER_HURT);
    else kill();
}

void Peach :: doSomething()
{
    if(isAlive() == false){return;}
    if(invincible>) invincible--;
    if(time_to_recharge_before_next_fire)time_to_recharge_before_next_fire--;
    int keymove;
    double x = getX(),y = getY();
    bool hasblockat = getWorld()->hasblock(x, y);
    if(jumpdistance){
        int estimate_y = y + 4;
        if(getWorld()->hasblock(x, estimate_y)){
            getWorld()->bonkat(x, estimate_y);
            jumpdistance = 0;
        }
        else{
            moveTo(x, estimate_y);
            jumpdistance--;
        }
        return;
    }
    else{
        int estimate_y = y-4;
        if(!getWorld()->hasblock(x, estimate_y)) moveTo(x, estimate_y);
    }
    getWorld()->getKey(keymove);
    switch(keymove)
    {
        case KEY_PRESS_LEFT:
            setDirection(180);x-=4;
            if(!getWorld()->hasblock(x,y))  moveTo(x, y);
            else
                getWorld()->bonkat(x, y);
            break;
        case KEY_PRESS_RIGHT:
            setDirection(0);
            x+=4;
            if(!getWorld()->hasblock(x,y))  moveTo(x, y);
            else
                getWorld()->bonkat(x, y);
            break;
        case KEY_PRESS_UP:
            if(getWorld()->hasblock(x, y-1)){
                if(!jump_power)jumpdistance = 8;
                else jumpdistance = 12;
                getWorld()->playSound(SOUND_PLAYER_JUMP);
            }
            break;
        case KEY_PRESS_SPACE:
            if(shoot_power!=0 && time_to_recharge_before_next_fire == 0){
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                time_to_recharge_before_next_fire = 8;
                if(!getDirection()){
                    getWorld()->addactor(new Peach_fireball(getWorld(), x+4, y, 0));    //fireballs will sfhit 4 pixels left/rightward
                }
                else{
                    getWorld()->addactor(new Peach_fireball(getWorld(), x-4, y, 180));
                }
            }
            break;
    }
}


//blocks
//------------------------------------------------
Block :: Block(sw* world, int startX, int startY, int ggoodie) : Actor(world, IID_BLOCK, startX, startY, 0, 2, 1){type_of_goodie = ggoodie;}

void Block :: bonked()
{
    if(type_of_goodie == 0)
        getWorld()->playSound(SOUND_PLAYER_BONK);
    else{
        getWorld()->playSound(SOUND_POWERUP_APPEARS);
        switch(type_of_goodie){
            case 1: getWorld()->addactor(new Flower(getWorld(), getX(), getY()+SPRITE_HEIGHT)); break;
            case 2: getWorld()->addactor(new Mushroom(getWorld(), getX(), getY()+SPRITE_HEIGHT));break;
            case 3: getWorld()->addactor(new Mushroom(getWorld(), getX(), getY()+SPRITE_HEIGHT)); break;
        }
        type_of_goodie = 0;
    }
}


void Block :: doSomething(){}


//pipe class
Pipe :: Pipe(sw* w, int startX, int startY) : Actor(w, IID_PIPE, startX, startY, 0, 2, 1){}
void Pipe :: bonked(){
    getWorld()->playSound(SOUND_PLAYER_BONK);
}

void Pipe :: doSomething(){}


//flags, marios
Flags :: Flags(sw* w, int startX, int startY) : Actor(w, IID_FLAG, startX, startY, 0, 1, 1){}

void Flags :: doSomething(){
    if(!isAlive())return;
    if(getWorld()->overlap_peach(getX(), getY())){
        kill();
        getWorld()->increaseScore(1000);
        getWorld()->level_complete();
    }
}

Mario :: Mario(sw* w, int startX, int startY) : Actor(w, IID_MARIO, startX, startY, 0, 1, 1){}

void Mario :: doSomething(){
    if(!isAlive())return;
    if(getWorld()->overlap_peach(getX(), getY()))
    {
        kill();
        getWorld()->increaseScore(1000);
        getWorld()->finish_game();
    }
}


//goodies
Flower :: Flower(sw* w, int startX, int startY) : Actor(w, IID_FLOWER, startX, startY, 0, 1, 1){}

void Flower :: doSomething(){
    int x = getX(), y = getY();
    if(getWorld()->overlap_peach(x, y))
    {
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        getWorld()->get_shoot_power();
        getWorld()->increaseScore(50);
        getWorld()->setpeachhp();
        kill();
        return;
    }
    if(!getWorld()->hasblock(x, y-2)) y-=2;
    moveTo(x, y);
    int curdir = getDirection();
    //left or right dir
    if(!curdir){
        int estimate_x = x+2;
        if(getWorld()->hasblock(estimate_x, y)){
            setDirection(180);
            return;
        }
        else{
            moveTo(estimate_x, y);
            return;
        }
    }
    else if (curdir==180){
        int estimate_x = x-2;
        if(getWorld()->hasblock(estimate_x, y)){
            setDirection(0);
            return;
        }
        else{
            moveTo(estimate_x, y);
            return;
        }
    }
}



Mushroom :: Mushroom(sw* w, int startX, int startY) : Actor(w, IID_MUSHROOM, startX, startY, 0, 1, 1){}

void Mushroom :: doSomething()
{
    int x = getX(), y = getY();
    if(getWorld()->overlap_peach(x, y))
    {
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        getWorld()->increaseScore(75);
        getWorld()->get_jump_power();
        getWorld()->setpeachhp();
        kill();
        return;
    }
    if(!getWorld()->hasblock(x, y-2)) y-=2;
    moveTo(x, y);
    int dir = getDirection();
    if(!dir){
        int estimate_x = x+2;
        if(getWorld()->hasblock(estimate_x, y)){
            setDirection(180);
            return;
        }
        else    moveTo(estimate_x, y);
    }
    else if(dir == 180){
        int estimate_x = x-2;
        if(getWorld()->hasblock(estimate_x, y))
        {
            setDirection(0);
            return;
        }
        else   moveTo(estimate_x, y);
    }
}

Star :: Star(sw* w, int startX, int startY) : Actor(w, IID_STAR, startX, startY, 0, 1, 1){}

void Star :: doSomething()
{
    int x = getX(), y = getY();
    if(getWorld()->overlap_peach(x, y))
    {
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        getWorld()->get_star_power();
        getWorld()->increaseScore(100);
        getWorld()->setpeachhp();
        kill();
        return;
    }
    if(!getWorld()->hasblock(x, y-2)) y-=2;
    moveTo(x, y);
    int dir = getDirection();
    if(!dir){
        int estimate_x = x+2;
        if(getWorld()->hasblock(estimate_x, y)){
            setDirection(180);
            return;
        }
        else    moveTo(estimate_x, y);
    }
    else if(dir == 180){
        int estimate_x = x-2;
        if(getWorld()->hasblock(estimate_x, y)){
            setDirection(0);
            return;
        }
        else   moveTo(estimate_x, y);
    }
}

Goomba :: Goomba(sw* w, int startX, int startY) : Actor(w, IID_GOOMBA, startX, startY, 0, 0, 1){}

void Goomba :: bonked(){
    if(getWorld()->getPeach()->hasinv()){
        getWorld()->playSound(SOUND_PLAYER_KICK);
        getWorld()->increaseScore(100);
        kill();
    }
    else{
        getWorld()->getPeach()->bonked();
    }
}


void Goomba :: doSomething()
{
    if(isAlive() == false)return;
    int x = getX(), y = getY();
    //bonked calculates the outcome of collision of the two
    if(getWorld()->overlap_peach(x, y)){
        bonked();
        return;
    }
    int dir = getDirection();
    if(!dir){
        if(getWorld()->hasblock(x+1, y) || !getWorld()->hasblock(x+SPRITE_WIDTH, y-1))  setDirection(180);
    }
    else{
        if(getWorld()->hasblock(x-1, y) || !getWorld()->hasblock(x-SPRITE_WIDTH, y-1))  setDirection(0);
    }
    if(!getDirection()){
        if(getWorld()->hasblock(x+1, y)) return;
        else{
            moveTo(x+1, y);
            return;
        }
    }
    else if(getDirection() == 180){
        if(getWorld()->hasblock(x-1, y)) return;
        else{
            moveTo(x-1, y);
            return;
        }
    }
}

//koopa class
Koopa :: Koopa(sw* w, int startX, int startY) : Actor(w, IID_KOOPA, startX, startY, 0, 0, 1){}

void Koopa :: bonked(){
    if(getWorld()->getPeach()->hasinv()){
        getWorld()->playSound(SOUND_PLAYER_KICK);
        getWorld()->increaseScore(100);
        kill();
        getWorld()->addactor(new Shell(getWorld(), getX(), getY(), getDirection()));
    }
    else getWorld()->getPeach()->bonked();
}

void Koopa :: doSomething()
{
    if(isAlive() == false)return;
    int x = getX();
    int y = getY();
    if(getWorld()->overlap_peach(x, y)){
        bonked();
        return;
    }
    int dir = getDirection();
    if(!dir){
        if(getWorld()->hasblock(x+1, y) || !getWorld()->hasblock(x+SPRITE_WIDTH, y-1)) setDirection(180);
    }
    else{
        if(getWorld()->hasblock(x-1, y) || !getWorld()->hasblock(x-SPRITE_WIDTH, y-1)) setDirection(0);
    }
    if(!getDirection()){
        if(getWorld()->hasblock(x+1, y))    return;
        else{
            moveTo(x+1, y);
            return;
        }
    }
    else if(getDirection() == 180){
        if(getWorld()->hasblock(x-1, y)) return;
        else{
            moveTo(x-1, y);
            return;
        }
    }
}


//piranha
Piranha :: Piranha(sw* w, int startX, int startY) : Actor(w, IID_PIRANHA, startX, startY, 0, 0, 1){}


void Piranha :: bonked()
{
    if(getWorld()->getPeach()->hasinv()){
        getWorld()->playSound(SOUND_PLAYER_KICK);
        getWorld()->increaseScore(100);
        kill();
    }
    else{
        getWorld()->getPeach()->bonked();
    }
}

void Piranha :: doSomething(){
    if(isAlive() == false)return;
    increaseAnimationNumber();
    int x = getX(), y = getY();
    if(getWorld()->overlap_peach(x, y)){
        bonked();
        return;
    }
    int dist = y-getWorld()->getPeach()->getY();
    dist<0?dist*=-1:dist*=1;    
    if(dist>1.5*SPRITE_HEIGHT)  return;               //check whether in range
    if(x>getWorld()->getPeach()->getX())
        setDirection(180);
    else
        setDirection(0);
    if(fire_delay){
        fire_delay--;
        return;
    }
    else{
        int dist2 = x-getWorld()->getPeach()->getX();
        dist2<0?dist2*=-1:dist2*=1;
        if(dist2<8*SPRITE_WIDTH){
            getWorld()->playSound(SOUND_PIRANHA_FIRE);
            getWorld()->addactor(new Piranha_fireball(getWorld(), x, y, getDirection()));
            fire_delay = 40;
        }
    }
}


//piranha fireball class
Piranha_fireball :: Piranha_fireball(sw* w, int startX, int startY, int direction) : Actor(w, IID_PIRANHA_FIRE, startX, startY, direction, 1, 1){dir = direction;}

void Piranha_fireball :: doSomething(){
    int x = getX(), y = getY();
    //if overlaps with peach, directly exit program
    if(getWorld()->overlap_peach(x, y)){
        kill();
        getWorld()->bonkpeach();
        return;
    }
    if(!getWorld()->hasblock(x, y-2)) y-=2;
    moveTo(x, y);
    if(!dir){
        if(getWorld()->hasblock(x+2, y)){
            kill();
            return;
        }
        else{
            moveTo(x+2, y);
            return;
        }
    }
    else if(dir == 180){
        if(getWorld()->hasblock(x-2, y)){
            kill();
            return;
        }
        else{
            moveTo(x-2, y);
            return;
        }
    }
}

//peach fireball
Peach_fireball :: Peach_fireball(sw* w, int startX, int startY, int direction) : Actor(w, IID_PEACH_FIRE, startX, startY, direction, 1, 1){dir = direction;}

void Peach_fireball :: doSomething()
{
    int x = getX(), y = getY();
    //if overlap with an enemy, return
    if(getWorld()->damage(x, y)){
        kill();
        getWorld()->damageat(x, y);
        return;
    }
    if(!getWorld()->hasblock(x, y-2)) y-=2;
    moveTo(x, y);
    if(!dir){
        if(getWorld()->hasblock(x+2, y)){
            kill();
            return;
        }
        else{
            moveTo(x+2, y);
            return;
        }
        
    }
    else if(dir == 180){
        if(getWorld()->hasblock(x-2, y)){
            kill();
            return;
        }
        else{
            moveTo(x-2, y);
            return;
        }
    }
}

//shell class
Shell :: Shell(sw* w, int startX, int startY, int direction) : Actor(w, IID_SHELL, startX, startY, direction, 1, 1){dir = direction;}

void Shell :: doSomething(){
    int x = getX(), y = getY();
    if(getWorld()->damage(x, y)){
        getWorld()->damageat(x, y);             //shell will damage  any enemy object
        kill();
        return;
    }
    if(!getWorld()->hasblock(x, y-2)) y-=2;
    moveTo(x, y);
    if(!dir){
        if(getWorld()->hasblock(x+2, y)){       
            kill();
            return;
        }
        else{
            moveTo(x+2, y);
            return;
        }
        
    }
    else if(dir == 180){
        if(getWorld()->hasblock(x-2, y)){
            kill();
            return;
        }
        else{
            moveTo(x-2, y);
            return;
        }
    }
}
