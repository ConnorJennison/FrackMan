#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <list>
#include <stack>
#include <algorithm>

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Dirt;
class FrackMan;
class Protestor;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    
    //For Frackman, check if you remove dirt, and if so, play the digging sound
    bool dig(int x, int y);
    
    //RemoveDirt around an object
    void removeDirt(int x, int y);
    
    
    //See if an actor exists at these coordinates
    bool findActor(Actor* ap, int x ,int y);
    
    //Is there any dirt under the boulder?
    bool dirtUnderBoulder(int x, int y);
    
    //Check if a proposed x,y falls within the mineshaft
    bool inMineshaft(int x);
    
    //get a random number in range
    int random(int begin, int end);
    
    //Checks if an actor can move here
    bool canActorMoveTo(Actor* ap, int x, int y) const;
    
    //Check to see if two objects are within a certain
    bool withinRadius(double rad, double x1, double y1, double x2, double y2) const;
    
    //Annoy any nearby actors that can be annoyed
     int annoyAllNearbyActors(Actor* annoyer, int points, int radius);
    
    //Annoy any nearby protestors that can be annoyed
    int annoyAllNearbyProtestors(Actor* annoyer, int points, int radius);
    
    //Create Boulders
    void initBoulders(int level);
    
    //Find frackman's x position
    int getFrackX() const;
    
    //Find frackman's y position
    int getFrackY() const;
    
    //Initialize the gamespace
    virtual int init();
    
    //Complete one turn
    virtual int move();
    
    //Clean up the gamespace
    virtual void cleanUp();
    
    //Check if inbetween
    bool between(int check, int varbegin, int varend) const;
    
    //Add a squirt if possible to an (x,y)
    void addSquirt(int x, int y, Actor::Direction dir);
    
    //Add a gold nugget
    void addGold(int x, int y);
    
    //Remove one possible objective
    void removeOilObjective();
    
    //Return how many oils are left
    int oilLeft();
    
    //Set # of oil objects at beginning of level
    void initOilNumber(int level);
    
    //Initialize the oil
    void initOil();
    
    //Give Frackman a sonar
    void giveFrackSonar();
    
    //Give Frackman 5 squirts
    void giveFrackSquirts();
    
    //GiveFrackman gold
    void giveFrackGold();
    
    //Find a protestor nearby.
    Protestor* findNearbyProtestor(Actor* a, int radius) const;
    
    //Initialize the Gold
    void initGold(int level);
    
    //Add goodies
    void addGoodies(int level);
    
    //See if a dirt section is empty
    bool clearDirt(int x, int y) const ;
    
    //Update the game text
    void updateGameText();
    
    //Use a sonar kit
    void useSonarKit();
    
    //See if facing towards frackman
    bool facingTowardsFrackman(Actor* a);
    
    //See if in line of sight to frackman
    GraphObject::Direction lineofSighttoFrackman(Actor* a) const;
    
    //Return pointer to frackman
    Actor* getFrackman();
    
    
    
    
    

private:
    //Dirt objects
    list<Dirt*> m_dirt;
    
    //Protestor objects
    list<Protestor*> m_protestors;
    
    //Other objects
    list<Actor*> m_gameobjects;
    
    //FRACKMAN
    FrackMan* m_frackman;
    
    //Number of barrells in a level
    int m_oilLeft;
    
    //Max number of protestors in a field
    int maxProtestors;
    
    //Number of ticks since last protestor was added.
    int ticksUntilNewProtestor;
};

#endif // STUDENTWORLD_H_


