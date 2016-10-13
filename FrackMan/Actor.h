#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;


class Actor: public GraphObject
{
public:
    Actor(StudentWorld* world,int imageID, int startX, int startY, Direction dir, double size , unsigned int depth, unsigned int health);
    virtual ~Actor();
    
    //perform an action
    virtual void doSomething() = 0;
    
    //annoy this actor
    virtual bool annoy(unsigned int amt);
    
    //check if alive
    bool isAlive();
    
    //return the world
    StudentWorld* getWorld() {return m_world;};
    
    //set to dead
    void setDead();
    
    //Does this need to be picked up to finish the level?
    virtual bool needsToBePickedUpToFinishLevel() {return false;};
    
    //Can other actors pass through this object
    virtual bool canActorsPassThroughMe() const {return true;};
    
    //Can this actor dig through dirt
    virtual bool canDigThroughDirt() const {return false;};
    
    //Return m_hp
    int getHP();

private:
    unsigned int m_hp;
    StudentWorld* m_world;
    bool m_alive = true;
 
};




class FrackMan: public Actor
{
public:
    //constructor
    FrackMan(StudentWorld* world);
    
    //Process one turn
    virtual void doSomething();
    
    //destroy
    virtual ~FrackMan();
    
    //Can this actor dig through dirt
    virtual bool canDigThroughDirt() const {return true;};
    
    //Give FrackMan a sonarKit
    void giveSonar();
    
    //Give FrackMan 5 squirts
    void giveSquirts();
    
    //Give FrackMan a gold
    void giveGold();
    
    //Return how many squirts are left
    int squirtsLeft();
    
    //Return how many gold frackman has
    int goldLeft();
    
    //Return how many sonars frackman has
    int sonarsLeft();
    
private:
    //amount of squirts left in inventory
    unsigned int m_squirts;
    
    //amount of sonars left in inventory
    unsigned int m_sonars;
    
    //amount of nuggets left in inventory
    unsigned int m_nuggets;
};




class Protestor: public Actor
{
public:
    //constructor
    Protestor(StudentWorld* world, int imageID, int startX, int startY, unsigned int hp);
    
    //process one turn
    virtual void doSomething() = 0;
    
    //deconstruct
    virtual ~Protestor();
    
    //return the amount of waiting ticks left
    int getWaitingTicks();
    
    //Reset waiting ticks to the proper value
    void resetWaitingTicks();
    
    //Decrease waiting ticks
    void decWaitingTicks();
    
    //Check if in a leaving state
    bool isLeaving() const;
    
    //Set the yelling ticks
    void setYellingTicks();
    
    //Get the yelling ticks
    int getYellingTicks() const;
    
    //annoy this actor
    virtual bool annoy(unsigned int amt);
    
    //Bribe the protestor
    void bribeProtestor();
    
    //Return whether or not enough ticks have elapsed since last yell
    bool canYell() const;
    
    //Sets the number of squares for the protester to move before checking
    void setNumSquaresToMove(int squares);
    
    //Return number of squares he is moving
    int getNumSquaresToMove() const;
    
    //Set ticks until he can make a perpendicular move
    void setPerpendicularTicks();
    
    //Return that number
    int getPerpendicularTicks() const;
    
    //Decrease ticks that were not used in a turn
    void decOtherTicks();
    
    //Return if a protestor is leaving or not
    bool canAnnoy() const;
    
    //Hurts the protestor, but does not kill/set leaving
    void stunProtestor();
    
    //Set the protestor to be leaving.
    void setLeaving();
    
    //Returns whether or not gold can set a protestor to leave.
    virtual bool canBeBribed() const {return true;};
    
    
    
    
    
private:
    bool leaveState;
    int waitingTicks;
    int numSquaresToMoveInCurrentDirection;
    int TicksUntilCanYellAgain;
    int perpendicularTicks;
 
    
};

class NormalProtestor: public Protestor
{
public:
    //constructor
    NormalProtestor(StudentWorld* world, int startX, int startY);
    
    //destructor
    virtual ~NormalProtestor();
    
    void setNewDir();
    
    //process one turn
    virtual void doSomething();
};



class HardcoreProtestor: public Protestor
{
public:
    //constructor
    HardcoreProtestor(StudentWorld* world, int startX, int startY);
    
    virtual ~HardcoreProtestor();
    
    void setNewDir();
    
    //process one turn
    virtual void doSomething();
    
    virtual bool canBeBribed() const {return false;};
    
    
private:
    
};


class Boulder: public Actor
{
public:
    //constructor
    Boulder(StudentWorld* world, int startX, int startY);
    
    //process one turn
    virtual void doSomething();
    
    //destroy
    virtual ~Boulder();
    
    //return if stable
    bool isStable() {return stableState;};
    
    //return if in waiting state
    bool isWaiting() {return waitingState;};
    
    //return if in falling state
    bool isFalling() {return fallingState;};
    
    //set to waiting state
    void setWaiting()
    {
        stableState = false;
        waitingState = true;
    };
    
    //set to falling state
    void setFalling()
    {
        waitingState = false;
        fallingState = true;
    };
    
    //Can other actors pass through this object
    virtual bool canActorsPassThroughMe() const {return false;};
    
private:
    bool stableState;
    bool waitingState;
    bool fallingState;
    
    //elapsed ticks that have occured in waiting state
    unsigned int elapsedTicks;
    
    
};


class Squirt: public Actor
{
public:
    //constructor
    Squirt(StudentWorld* world, int startX, int startY, Direction startDir);
    
    //Process one turn
    virtual void doSomething();
    
    //destory
    virtual ~Squirt();
private:
    int m_travelDistance;
};


class Goodie : public Actor
{
public:
    Goodie(StudentWorld* world, int imageID, int startX, int startY,
                     int soundToPlay, bool activateOnPlayer,
                     bool activateOnProtester, bool initallyActive);
    
    //process one turn
    virtual void doSomething() {return;};
    
    //destructor
    virtual ~Goodie();
    
    //see if active
    bool isActive() {return m_active;};
    
    //set active
    void setActive() {m_active = true;};
    
    //return if it needs to be picked up to end the level
    virtual bool needsToBePickedUpToEndLevel() {return false;};
    
    virtual void setTicks() {return;};
    
    bool FrackManCanPickup();
    
private:
    int soundToPlay;
    bool m_activateFrackMen;
    bool m_activateProtestor;
    bool m_active;
};




class Oil: public Goodie
{
public:
    //constructor
    Oil(StudentWorld* world, int startX, int startY);
    
    //process one turn
    virtual void doSomething();
    
    //destroy
    virtual ~Oil();
    
    //This needs to be picked up to finish the level
    virtual bool needsToBePickedUpToFinishLevel() {return true;};
private:
    
};



class Gold: public Goodie
{
public:
    //constructor
    Gold(StudentWorld* world, int startX, int startY, bool protestorCanPickup);
    
    //process one turn
    virtual void doSomething();
    
    virtual void setTicks();
    
    //destory
    virtual ~Gold();
private:
    int TicksLeft;
    
    
};


class SonarKit: public Goodie
{
public:
    //constructor
    SonarKit(StudentWorld* world, int startX, int startY);
    
    //process one turn
    virtual void doSomething();
    
    //destroy
    virtual ~SonarKit();
    
    //set ticks left to live
    virtual void setTicks();
private:
    int TicksLeft;
    
};

class WaterRefill: public Goodie
{
public:
    //constructor
    WaterRefill(StudentWorld* world, int startX, int startY);
    
    //process one turn
    virtual void doSomething();
    
    //destroy
    virtual ~WaterRefill();
    
    virtual void setTicks();
private:
    int TicksLeft;
};

class Dirt: public Actor
{
public:
    //constructor
    Dirt(StudentWorld* world, int x, int y);
    
    //process one turn
    virtual void doSomething() {return;};
    
    //destroy
    virtual ~Dirt();
    
    //Can other actors pass through this object
    virtual bool canActorsPassThroughMe() const {return false;};

private:
};





#endif // ACTOR_H_
