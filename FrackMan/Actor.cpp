
#include "StudentWorld.h"
#include <cmath>

using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


//////////////////////////////////////////////////////
///////                                    ///////////
///////                ACTOR               ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

Actor::Actor(StudentWorld* world,int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, unsigned int health): GraphObject(imageID,startX, startY, dir, size, depth), m_hp(health)
{
    setVisible(true);
    m_world = world;
}

Actor::~Actor() //Deconstruct, set to invisible
{
    setVisible(false);
}

bool Actor::isAlive() //If hp is 0, return alive
{
    return m_alive;
}


void Actor::setDead()
{
    m_alive = false;
}

bool Actor::annoy(unsigned int amt)
{
    if (m_hp <= amt)
    {
        m_hp = 0;
        setDead();
        return true;
    }
    
    else
    {
        m_hp-=amt;
        return false;
    }
}

int Actor::getHP()
{
    return m_hp;
}




//////////////////////////////////////////////////////
///////                                    ///////////
///////                FRACKMAN            ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

FrackMan::FrackMan(StudentWorld* world): Actor(world, IID_PLAYER, 30, 60, right, 1.0, 0, 10)
{
    m_squirts = 5;
    m_nuggets = 0;
    m_sonars = 1;
    setVisible(true);
}

void FrackMan::doSomething() //BAD EXEC
{
    //CHECK IF ALIVE//
 
    if (!isAlive())
        return;

    //CHECK IF OVERLAPPING ANY DIRT//

    if (getWorld()->dig(getX(), getY()))
        getWorld()->playSound(SOUND_DIG);
        
    
    //TRY TO MOVE IN A CERTAIN DIRECTION
    
    int ch;
    const int Z = 'Z';
    const int z = 'z';
    if (getWorld()->getKey(ch) == true)
    {
        Direction dir = getDirection();
        switch (ch)
        {
            //IF PRESSED ARROW, MOVE/FACE THAT DIRECTION
            case KEY_PRESS_LEFT:
                if (dir == left && getWorld()->canActorMoveTo(this, getX() - 1, getY()))
                    moveTo(getX() - 1, getY());
                setDirection(left);
                break;
            case KEY_PRESS_RIGHT:
                if (dir == right && getWorld()->canActorMoveTo(this, getX() + 1, getY()))
                    moveTo(getX() + 1, getY());
                setDirection(right);
                break;
            case KEY_PRESS_UP:
                if (dir == up && getWorld()->canActorMoveTo(this, getX(), getY() + 1))
                    moveTo(getX(), getY() + 1);
                setDirection(up);
                break;
            case KEY_PRESS_DOWN:
                    if (dir == down && getWorld()->canActorMoveTo(this, getX(), getY()-1))
                    moveTo(getX(), getY() - 1);
                setDirection(down);
                break;
                
           //IF PUSHED SPACE, FIRE A SQUIRT
            case KEY_PRESS_SPACE:
                if (m_squirts > 0)
                {
                    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                    m_squirts--;
                    
                    switch (dir)
                    {
                        case up:
                            getWorld()->addSquirt(getX(), getY() + 4, up);
                            break;
                        case down:
                            getWorld()->addSquirt(getX(), getY() - 4, down);
                            break;
                        case left:
                            getWorld()->addSquirt(getX() - 4, getY(), left);
                            break;
                        case right:
                            getWorld()->addSquirt(getX() + 4, getY(), right);
                            break;
                        default:
                            return;
                    }
                }
                break;
            //IF PRESSED TAB, DROP A GOLD NUGGET
            case KEY_PRESS_TAB:
                if (m_nuggets > 0)
                {
                    m_nuggets--;
                    getWorld()->addGold(getX(), getY());
                }
                break;
            //IF PRESSED Z, USE A SONAR KIT
            case Z:
            case z:
                if (m_sonars > 0)
                {
                    getWorld()->playSound(SOUND_SONAR);
                    m_sonars--;
                    getWorld()->useSonarKit();
                }
                break;
            case KEY_PRESS_ESCAPE:
                annoy(100);
                break;
        }
    }
}


FrackMan::~FrackMan()
{
    setVisible(false);
}

void FrackMan::giveSonar()
{
    m_sonars++;
}

void FrackMan::giveSquirts()
{
    m_squirts += 5;
}

void FrackMan::giveGold()
{
    m_nuggets++;
}

int FrackMan::squirtsLeft()
{
    return m_squirts;
}

int FrackMan::goldLeft()
{
    return m_nuggets;
}

int FrackMan::sonarsLeft()
{
    return m_sonars;
}



//////////////////////////////////////////////////////
///////                                    ///////////
///////                DIRT                ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

Dirt::Dirt(StudentWorld* world, int x, int y): Actor(world, IID_DIRT, x, y, right, 0.25, 3, 0)
{
    setVisible(true);
    
}

Dirt::~Dirt()
{
    setVisible(false);
}


//////////////////////////////////////////////////////
///////                                    ///////////
///////              BOULDER               ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

Boulder::Boulder(StudentWorld* world, int startX, int startY): Actor(world, IID_BOULDER, startX, startY, down, 1, 3, 0)
{
    stableState = true;
    waitingState = false;
    fallingState = false;
    elapsedTicks = 0;
};

Boulder::~Boulder()
{
    setVisible(false);
}

void Boulder::doSomething()
{
    //CHECK IF ALIVE//
    
    if (!isAlive())
        return;
    
    getWorld()->removeDirt(getX(), getY());
    
    //STABLE STATE//
    
    if (isStable())
    {
        //If there is dirt under, do nothing//
        if (getWorld() -> dirtUnderBoulder(getX(), getY()))
            return;
        //Otherwise, enter waiting state//
        else
        {
            setWaiting();
            return;
        }
    }
    
    
    //WAITING STATE//
    
    if (isWaiting())
    {
        if (elapsedTicks < 30)
        {
            elapsedTicks++;
            return;
        }
        
        else
        {
            setFalling();
            getWorld()->playSound(SOUND_FALLING_ROCK);
            return;
        }
    }
    
    
    //FALLING STATE //
    
    if (isFalling())
    {
        //CONTINUE DOWNWARD UNTIL REACHING THE BOTTOM
        if (getWorld()->canActorMoveTo(this, getX(), getY()-1))
            moveTo(getX(), getY()-1);
        else
        {
            setDead();
            return;
        }
    
        
        //CHECK FOR PROTESTORS AND FRACKMAN IN RADIUS//
        getWorld()->annoyAllNearbyActors(this, 100, 3.0);
    }
    
    
    return;
    
}






//////////////////////////////////////////////////////
///////                                    ///////////
///////              PROTESTOR             ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

Protestor::Protestor(StudentWorld* world, int imageID, int startX, int startY, unsigned int hp): Actor(world, imageID, startX, startY, left, 1.0, 0, hp)
{
    setVisible(true);
    waitingTicks = 0;
    leaveState = false;
    TicksUntilCanYellAgain = 0;
    perpendicularTicks = 0;
}

Protestor::~Protestor()
{
    setVisible(false);
}

int Protestor::getWaitingTicks()
{
    return waitingTicks;
}

void Protestor::resetWaitingTicks()
{
    int level = getWorld()->getLevel();
    waitingTicks = max(0, 3-(level/4));
}

void Protestor::decWaitingTicks()
{
    waitingTicks--;
}

bool Protestor::isLeaving() const

{
    return leaveState;
}

bool Protestor::canYell() const
{
    return TicksUntilCanYellAgain == 0;
}

void  Protestor::setYellingTicks()
{
    TicksUntilCanYellAgain = 15;
}

int Protestor::getYellingTicks() const
{
    return TicksUntilCanYellAgain;
}

void Protestor::setNumSquaresToMove(int squares)
{
    numSquaresToMoveInCurrentDirection = squares;
}

int Protestor::getNumSquaresToMove() const
{
    return numSquaresToMoveInCurrentDirection;
}

void Protestor::setPerpendicularTicks()
{
    perpendicularTicks = 200;
}

int Protestor::getPerpendicularTicks() const
{
    return perpendicularTicks;
}

void Protestor::decOtherTicks()
{
    if (perpendicularTicks != 0)
        perpendicularTicks--;
    if (TicksUntilCanYellAgain != 0)
        TicksUntilCanYellAgain--;
}

void Protestor::stunProtestor()
{
    int level = getWorld()->getLevel();
    waitingTicks = max(50,100-level*10);
}

bool Protestor::canAnnoy() const
{
    if (isLeaving())
        return false;
    else
        return true;
}

bool Protestor::annoy(unsigned int amt)
{
    if (Actor::annoy(amt))
    {
        setLeaving();
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        return true;
        
    }
    else
    {
        stunProtestor();
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        return false;
    }
}

void Protestor::setLeaving()
{
    leaveState = true;
    waitingTicks = 0;
}

void Protestor::bribeProtestor()
{
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    if (canBeBribed())
    {
        getWorld()->increaseScore(25);
        setLeaving();
    }
    else
    {
        getWorld()->increaseScore(50);
        stunProtestor();
    }
}



//////////////////////////////////////////////////////
///////                                    ///////////
///////            NORMAL PROTESTOR        ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

NormalProtestor::NormalProtestor(StudentWorld* world, int startX, int startY): Protestor(world, IID_PROTESTER, startX, startY, 5)
{
    setVisible(true);
}

NormalProtestor::~NormalProtestor()
{
    setVisible(false);
}

void NormalProtestor::setNewDir()
{
    bool foundGoodDir = false;
    while (foundGoodDir != true)
    {
        int dir = getWorld()->random(1, 4);
        switch (dir)
        {
            case 1: //UP
                if (!(getWorld()->canActorMoveTo(this,getX(), getY() + 1)))
                {
                    continue;
                }
                else
                {
                    setDirection(up);
                    int howMany = 0;
                    foundGoodDir = true;
                    for (int i = getY(); i <= 60; i++)
                    {
                        if (!getWorld()->canActorMoveTo(this,getX(), i))
                            break;
                        else
                            howMany++;
                        
                    }
                    
                    if (howMany <= 8)
                        setNumSquaresToMove(8);
                    else
                        setNumSquaresToMove(howMany);
                }
                break;
                
                
            case 2: //DOWN
                if (!(getWorld()->canActorMoveTo(this,getX(), getY() - 1)))
                {
                    continue;
                }
                else
                {
                    setDirection(down);
                    int howMany = 0;
                    foundGoodDir = true;
                    for (int i = getY(); i >= 0; i--)
                    {
                        if (!getWorld()->canActorMoveTo(this,getX(), i))
                            break;
                        else
                            howMany++;
                        
                    }
                    
                    if (howMany <= 8)
                        setNumSquaresToMove(8);
                    else
                        setNumSquaresToMove(howMany);
                }
                break;
                
                
            case 3: //LEFT
                if (!(getWorld()->canActorMoveTo(this,getX() -1 , getY())))
                {
                    continue;
                }
                else
                {
                    setDirection(left);
                    int howMany = 0;
                    foundGoodDir = true;
                    for (int i = getX(); i >= 0; i--)
                    {
                        if (!getWorld()->canActorMoveTo(this,i, getY()))
                            break;
                        else
                            howMany++;
                    }
                    
                    if (howMany <= 8)
                        setNumSquaresToMove(8);
                    else
                        setNumSquaresToMove(howMany);
                }
                break;
                
                
            case 4: //RIGHT
                if (!(getWorld()->canActorMoveTo(this,getX() + 1, getY())))
                {
                    continue;
                }
                else
                {
                    setDirection(right);
                    int howMany = 0;
                    foundGoodDir = true;
                    for (int i = getX(); i <= 60; i++)
                    {
                        if (!getWorld()->canActorMoveTo(this,i, getY()))
                            break;
                        else
                            howMany++;
                    }
                    
                    if (howMany <= 8)
                        setNumSquaresToMove(8);
                    else
                        setNumSquaresToMove(howMany);
                }
                break;
        }
        
    }

}



void NormalProtestor::doSomething()
{
    //IF DEAD, RETURN IMMEDEATELY//
    if (!isAlive())
        return;
    
    //IF RESTING, UPDATE RESTING TICK COUNT AND RETURN//
    if (getWaitingTicks() != 0)
    {
        decWaitingTicks();
        return;
    }
    
    //RESET THE WAITING TICKS
    resetWaitingTicks();
    
    
    //IF IT IS IN A LEVAING STATE, PROCESS//
    if (isLeaving())
    {
        setDead();
        return;
    }
   
    
    
    
    //IF WITHIN 4 UNITS OF FRACKMAN AND FACING FRACKMAN
    //IF IT HAS NOT SHOUTED AT THEM IN THE LAST 15 TICKS, PROCESS
    
    if (getWorld()->withinRadius(4.0, getWorld()->getFrackX(), getWorld()->getFrackY(), getX(), getY()))
        {
            if (canYell())
            {
                getWorld()->playSound(SOUND_PROTESTER_YELL);
                getWorld()->getFrackman()->annoy(2);
                decOtherTicks();
                setYellingTicks();
                return;
            }
        }
    
    
    
    
    
    
    //IF REGULAR PROTESTOR IS IN LINE OF SIGHT HORIZONTAL OR VERTICAL, MORE THAN 4 UNITS AWAY FROM FRACKMAN, CAN MOVE THE ENTIRE WAY TO FRACKMAN, PROCESS THAT STUFF
    
    if (!(getWorld()->withinRadius(4.0, getX(), getY(), getWorld()->getFrackX(), getWorld()->getFrackY())))
    {
        if (getWorld()->lineofSighttoFrackman(this) != GraphObject::none)
        {
            //Change direction, move, and set numsquaretomoveincurrentdir to 0;
            setDirection(getWorld()->lineofSighttoFrackman(this));
            GraphObject::Direction dir = getDirection();
            switch (dir)
            {
                case up:
                    moveTo(getX(), getY()+1);
                    break;
                case down:
                    moveTo(getX(), getY()-1);
                    break;
                case left:
                    moveTo(getX()-1, getY());
                    break;
                case right:
                    moveTo(getX()+1, getY());
                    break;
            }
            setNumSquaresToMove(0);
            decOtherTicks();
            return;
        }
    }
    
    
    //OTHERWISE PROTESTOR CANT SEE FRACKMAN, DECREASE NUMSQUARESTOMOVE BY ONE
    
    setNumSquaresToMove(getNumSquaresToMove() - 1);
    
    //IF THAT PARAMETER IS <= 0, FIND A NEW DIR
    if (getNumSquaresToMove() <= 0)
    {
        setNewDir();
    }
    

    
    //IF AT A PERPENDICULAR INTERSECTION && HAS NOT MOVED PERPENDICULAR WITHIN 200 TICKS
    
    else if (getNumSquaresToMove() > 0 && getPerpendicularTicks() == 0)
    {
        GraphObject::Direction dir = getDirection();
        switch (dir)
        {
            //EVALUATE FOR LEFT AND RIGHT
            case left:
            case right:
            {
                
            //Check up and down for viability
                bool canMoveUp = false;
                bool canMoveDown = false;
                
                if (getWorld()->canActorMoveTo(this, getX(), getY()+1))
                    canMoveUp = true;
                if (getWorld()->canActorMoveTo(this, getX(), getY()-1))
                    canMoveDown = true;
                
                if (canMoveDown && canMoveUp)
                //CASE WHERE YOU CAN SET BOTH DIRECTIONS
                {
                    setPerpendicularTicks();
                    int direction = getWorld()->random(1, 2);
                    switch (direction)
                    {
                        case 1:
                        {//UP
                                setDirection(up);
                                int howMany = 0;
                                
                                for (int i = getY(); i <= 60; i++)
                                {
                                    if (!getWorld()->canActorMoveTo(this,getX(), i))
                                        break;
                                    else
                                        howMany++;
                                    
                                }
                                
                                if (howMany <= 8)
                                    setNumSquaresToMove(8);
                                else
                                    setNumSquaresToMove(howMany);
                                break;
                        }
                        case 2: //DOWN
                        {
                            setDirection(down);
                            int howMany = 0;
                            for (int i = getY(); i >= 0; i--)
                            {
                                if (!getWorld()->canActorMoveTo(this,getX(), i))
                                    break;
                                else
                                    howMany++;
                                
                            }
                            
                            if (howMany <= 8)
                                setNumSquaresToMove(8);
                            else
                                setNumSquaresToMove(howMany);
                    
                            break;
                        }
                    }
                }
                
                else if (canMoveDown && !canMoveUp)
                {
                    setPerpendicularTicks();
                    setDirection(down);
                    int howMany = 0;
                    for (int i = getY(); i >= 0; i--)
                    {
                        if (!getWorld()->canActorMoveTo(this,getX(), i))
                            break;
                        else
                            howMany++;
                        
                    }
                    
                    if (howMany <= 8)
                        setNumSquaresToMove(8);
                    else
                        setNumSquaresToMove(howMany);
                }
                
                else if(canMoveUp && !canMoveDown)
                {
                    setPerpendicularTicks();
                    setDirection(up);
                    int howMany = 0;
                    
                    for (int i = getY(); i <= 60; i++)
                    {
                        if (!getWorld()->canActorMoveTo(this,getX(), i))
                            break;
                        else
                            howMany++;
                        
                    }
                    
                    if (howMany <= 8)
                        setNumSquaresToMove(8);
                    else
                        setNumSquaresToMove(howMany);
                }
                
                else
                {}
                
                break;
            }
                
            case up:
            case down:
            {
                //Check left and right for viability
                bool canMoveLeft = false;
                bool canMoveRight = false;
                
                if (getWorld()->canActorMoveTo(this, getX()-1, getY()))
                    canMoveLeft = true;
                if (getWorld()->canActorMoveTo(this, getX()+1, getY()))
                    canMoveRight = true;
                
                if (canMoveLeft && canMoveRight)
                    //CASE WHERE YOU CAN SET BOTH DIRECTIONS
                {
                    setPerpendicularTicks();
                    int direction = getWorld()->random(1, 2);
                    switch (direction)
                    {
                        case 1:
                        {//RIGHT
                            setDirection(right);
                            int howMany = 0;
                            
                            for (int i = getX(); i <= 60; i++)
                            {
                                if (!getWorld()->canActorMoveTo(this,i, getY()))
                                    break;
                                else
                                    howMany++;
                                
                            }
                            
                            if (howMany <= 8)
                                setNumSquaresToMove(8);
                            else
                                setNumSquaresToMove(howMany);
                            break;
                        }
                        case 2: //LEFT
                        {
                            setDirection(left);
                            int howMany = 0;
                            for (int i = getX(); i >= 0; i--)
                            {
                                if (!getWorld()->canActorMoveTo(this,i, getY()))
                                    break;
                                else
                                    howMany++;
                                
                            }
                            
                            if (howMany <= 8)
                                setNumSquaresToMove(8);
                            else
                                setNumSquaresToMove(howMany);
                            
                            break;
                        }
                    }
                }
                
                else if (canMoveLeft && !canMoveRight)
                {
                    setPerpendicularTicks();
                    setDirection(left);
                    int howMany = 0;
                    for (int i = getX(); i >= 0; i--)
                    {
                        if (!getWorld()->canActorMoveTo(this,i, getY()))
                            break;
                        else
                            howMany++;
                        
                    }
                    
                    if (howMany <= 8)
                        setNumSquaresToMove(8);
                    else
                        setNumSquaresToMove(howMany);
                }
                
                else if(canMoveRight && !canMoveLeft)
                {
                    setPerpendicularTicks();
                    setDirection(right);
                    int howMany = 0;
                    
                    for (int i = getX(); i <= 60; i++)
                    {
                        if (!getWorld()->canActorMoveTo(this,i, getY()))
                            break;
                        else
                            howMany++;
                        
                    }
                    
                    if (howMany <= 8)
                        setNumSquaresToMove(8);
                    else
                        setNumSquaresToMove(howMany);
                }
                
                else
                {}
                
                break;
            }
        }
    }
    
    //TRY TO MOVE IN THAT DIRECTION
    GraphObject::Direction move = getDirection();
    switch (move)
    {
        case up:
            if (getWorld()->canActorMoveTo(this, getX(), getY()+1))
            {
                moveTo(getX(), getY()+1);
                decOtherTicks();
            }
            else
            {
                setNumSquaresToMove(0);
                decOtherTicks();
            }
            break;
            
        case down:
            if (getWorld()->canActorMoveTo(this, getX(), getY()-1))
                {
                    moveTo(getX(), getY()-1);
                    decOtherTicks();
                }
            else
                {
                    setNumSquaresToMove(0);
                    decOtherTicks();
                }
            break;
        case left:
            if (getWorld()->canActorMoveTo(this, getX()-1, getY()))
            {
                moveTo(getX()-1, getY());
                decOtherTicks();
            }
            
            else
            {
                setNumSquaresToMove(0);
                decOtherTicks();
            }
            break;
            
        case right:
            if (getWorld()->canActorMoveTo(this,getX()+1, getY()))
            {
                moveTo(getX()+1, getY());
                decOtherTicks();
            }
            else
            {
                setNumSquaresToMove(0);
                decOtherTicks();
            }
    }
    
}






//////////////////////////////////////////////////////
///////                                    ///////////
///////         HARDCORE PROTESTOR         ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

HardcoreProtestor::HardcoreProtestor(StudentWorld* world, int startX, int startY): Protestor(world, IID_HARD_CORE_PROTESTER, startX, startY, 20)
{
    setVisible(true);
}

HardcoreProtestor::~HardcoreProtestor()
{
    setVisible(false);
}

void HardcoreProtestor::setNewDir()
{
    bool foundGoodDir = false;
    while (foundGoodDir != true)
    {
        int dir = getWorld()->random(1, 4);
        switch (dir)
        {
            case 1: //UP
                if (!(getWorld()->canActorMoveTo(this,getX(), getY() + 1)))
                {
                    continue;
                }
                else
                {
                    setDirection(up);
                    int howMany = 0;
                    foundGoodDir = true;
                    for (int i = getY(); i <= 60; i++)
                    {
                        if (!getWorld()->canActorMoveTo(this,getX(), i))
                            break;
                        else
                            howMany++;
                        
                    }
                    
                    if (howMany <= 8)
                        setNumSquaresToMove(8);
                    else
                        setNumSquaresToMove(howMany);
                }
                break;
                
                
            case 2: //DOWN
                if (!(getWorld()->canActorMoveTo(this,getX(), getY() - 1)))
                {
                    continue;
                }
                else
                {
                    setDirection(down);
                    int howMany = 0;
                    foundGoodDir = true;
                    for (int i = getY(); i >= 0; i--)
                    {
                        if (!getWorld()->canActorMoveTo(this,getX(), i))
                            break;
                        else
                            howMany++;
                        
                    }
                    
                    if (howMany <= 8)
                        setNumSquaresToMove(8);
                    else
                        setNumSquaresToMove(howMany);
                }
                break;
                
                
            case 3: //LEFT
                if (!(getWorld()->canActorMoveTo(this,getX() -1 , getY())))
                {
                    continue;
                }
                else
                {
                    setDirection(left);
                    int howMany = 0;
                    foundGoodDir = true;
                    for (int i = getX(); i >= 0; i--)
                    {
                        if (!getWorld()->canActorMoveTo(this,i, getY()))
                            break;
                        else
                            howMany++;
                    }
                    
                    if (howMany <= 8)
                        setNumSquaresToMove(8);
                    else
                        setNumSquaresToMove(howMany);
                }
                break;
                
                
            case 4: //RIGHT
                if (!(getWorld()->canActorMoveTo(this,getX() + 1, getY())))
                {
                    continue;
                }
                else
                {
                    setDirection(right);
                    int howMany = 0;
                    foundGoodDir = true;
                    for (int i = getX(); i <= 60; i++)
                    {
                        if (!getWorld()->canActorMoveTo(this,i, getY()))
                            break;
                        else
                            howMany++;
                    }
                    
                    if (howMany <= 8)
                        setNumSquaresToMove(8);
                    else
                        setNumSquaresToMove(howMany);
                }
                break;
        }
        
    }
    
}



void HardcoreProtestor::doSomething()
{
    //IF DEAD, RETURN IMMEDEATELY//
    if (!isAlive())
        return;
    
    //IF RESTING, UPDATE RESTING TICK COUNT AND RETURN//
    if (getWaitingTicks() != 0)
    {
        decWaitingTicks();
        return;
    }
    
    //RESET THE WAITING TICKS
    resetWaitingTicks();
    
    
    //IF IT IS IN A LEVAING STATE, PROCESS//
    if (isLeaving())
    {
        setDead();
        return;
    }
    
    
    
    
    //IF WITHIN 4 UNITS OF FRACKMAN AND FACING FRACKMAN
    //IF IT HAS NOT SHOUTED AT THEM IN THE LAST 15 TICKS, PROCESS
    
    if (getWorld()->withinRadius(4.0, getWorld()->getFrackX(), getWorld()->getFrackY(), getX(), getY()))
    {
        if (canYell())
        {
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            getWorld()->getFrackman()->annoy(2);
            decOtherTicks();
            setYellingTicks();
            return;
        }
    }
    
    
    
    
    
    
    //IF REGULAR PROTESTOR IS IN LINE OF SIGHT HORIZONTAL OR VERTICAL, MORE THAN 4 UNITS AWAY FROM FRACKMAN, CAN MOVE THE ENTIRE WAY TO FRACKMAN, PROCESS THAT STUFF
    
    if (!(getWorld()->withinRadius(4.0, getX(), getY(), getWorld()->getFrackX(), getWorld()->getFrackY())))
    {
        if (getWorld()->lineofSighttoFrackman(this) != GraphObject::none)
        {
            //Change direction, move, and set numsquaretomoveincurrentdir to 0;
            setDirection(getWorld()->lineofSighttoFrackman(this));
            GraphObject::Direction dir = getDirection();
            switch (dir)
            {
                case up:
                    moveTo(getX(), getY()+1);
                    break;
                case down:
                    moveTo(getX(), getY()-1);
                    break;
                case left:
                    moveTo(getX()-1, getY());
                    break;
                case right:
                    moveTo(getX()+1, getY());
                    break;
            }
            setNumSquaresToMove(0);
            decOtherTicks();
            return;
        }
    }
    
    
    //OTHERWISE PROTESTOR CANT SEE FRACKMAN, DECREASE NUMSQUARESTOMOVE BY ONE
    
    setNumSquaresToMove(getNumSquaresToMove() - 1);
    
    //IF THAT PARAMETER IS <= 0, FIND A NEW DIR
    if (getNumSquaresToMove() <= 0)
    {
        setNewDir();
    }
    
    
    
    //IF AT A PERPENDICULAR INTERSECTION && HAS NOT MOVED PERPENDICULAR WITHIN 200 TICKS
    
    else if (getNumSquaresToMove() > 0 && getPerpendicularTicks() == 0)
    {
        GraphObject::Direction dir = getDirection();
        switch (dir)
        {
                //EVALUATE FOR LEFT AND RIGHT
            case left:
            case right:
            {
                
                //Check up and down for viability
                bool canMoveUp = false;
                bool canMoveDown = false;
                
                if (getWorld()->canActorMoveTo(this, getX(), getY()+1))
                    canMoveUp = true;
                if (getWorld()->canActorMoveTo(this, getX(), getY()-1))
                    canMoveDown = true;
                
                if (canMoveDown && canMoveUp)
                    //CASE WHERE YOU CAN SET BOTH DIRECTIONS
                {
                    setPerpendicularTicks();
                    int direction = getWorld()->random(1, 2);
                    switch (direction)
                    {
                        case 1:
                        {//UP
                            setDirection(up);
                            int howMany = 0;
                            
                            for (int i = getY(); i <= 60; i++)
                            {
                                if (!getWorld()->canActorMoveTo(this,getX(), i))
                                    break;
                                else
                                    howMany++;
                                
                            }
                            
                            if (howMany <= 8)
                                setNumSquaresToMove(8);
                            else
                                setNumSquaresToMove(howMany);
                            break;
                        }
                        case 2: //DOWN
                        {
                            setDirection(down);
                            int howMany = 0;
                            for (int i = getY(); i >= 0; i--)
                            {
                                if (!getWorld()->canActorMoveTo(this,getX(), i))
                                    break;
                                else
                                    howMany++;
                                
                            }
                            
                            if (howMany <= 8)
                                setNumSquaresToMove(8);
                            else
                                setNumSquaresToMove(howMany);
                            
                            break;
                        }
                    }
                }
                
                else if (canMoveDown && !canMoveUp)
                {
                    setPerpendicularTicks();
                    setDirection(down);
                    int howMany = 0;
                    for (int i = getY(); i >= 0; i--)
                    {
                        if (!getWorld()->canActorMoveTo(this,getX(), i))
                            break;
                        else
                            howMany++;
                        
                    }
                    
                    if (howMany <= 8)
                        setNumSquaresToMove(8);
                    else
                        setNumSquaresToMove(howMany);
                }
                
                else if(canMoveUp && !canMoveDown)
                {
                    setPerpendicularTicks();
                    setDirection(up);
                    int howMany = 0;
                    
                    for (int i = getY(); i <= 60; i++)
                    {
                        if (!getWorld()->canActorMoveTo(this,getX(), i))
                            break;
                        else
                            howMany++;
                        
                    }
                    
                    if (howMany <= 8)
                        setNumSquaresToMove(8);
                    else
                        setNumSquaresToMove(howMany);
                }
                
                else
                {}
                
                break;
            }
                
            case up:
            case down:
            {
                //Check left and right for viability
                bool canMoveLeft = false;
                bool canMoveRight = false;
                
                if (getWorld()->canActorMoveTo(this, getX()-1, getY()))
                    canMoveLeft = true;
                if (getWorld()->canActorMoveTo(this, getX()+1, getY()))
                    canMoveRight = true;
                
                if (canMoveLeft && canMoveRight)
                    //CASE WHERE YOU CAN SET BOTH DIRECTIONS
                {
                    setPerpendicularTicks();
                    int direction = getWorld()->random(1, 2);
                    switch (direction)
                    {
                        case 1:
                        {//RIGHT
                            setDirection(right);
                            int howMany = 0;
                            
                            for (int i = getX(); i <= 60; i++)
                            {
                                if (!getWorld()->canActorMoveTo(this,i, getY()))
                                    break;
                                else
                                    howMany++;
                                
                            }
                            
                            if (howMany <= 8)
                                setNumSquaresToMove(8);
                            else
                                setNumSquaresToMove(howMany);
                            break;
                        }
                        case 2: //LEFT
                        {
                            setDirection(left);
                            int howMany = 0;
                            for (int i = getX(); i >= 0; i--)
                            {
                                if (!getWorld()->canActorMoveTo(this,i, getY()))
                                    break;
                                else
                                    howMany++;
                                
                            }
                            
                            if (howMany <= 8)
                                setNumSquaresToMove(8);
                            else
                                setNumSquaresToMove(howMany);
                            
                            break;
                        }
                    }
                }
                
                else if (canMoveLeft && !canMoveRight)
                {
                    setPerpendicularTicks();
                    setDirection(left);
                    int howMany = 0;
                    for (int i = getX(); i >= 0; i--)
                    {
                        if (!getWorld()->canActorMoveTo(this,i, getY()))
                            break;
                        else
                            howMany++;
                        
                    }
                    
                    if (howMany <= 8)
                        setNumSquaresToMove(8);
                    else
                        setNumSquaresToMove(howMany);
                }
                
                else if(canMoveRight && !canMoveLeft)
                {
                    setPerpendicularTicks();
                    setDirection(right);
                    int howMany = 0;
                    
                    for (int i = getX(); i <= 60; i++)
                    {
                        if (!getWorld()->canActorMoveTo(this,i, getY()))
                            break;
                        else
                            howMany++;
                        
                    }
                    
                    if (howMany <= 8)
                        setNumSquaresToMove(8);
                    else
                        setNumSquaresToMove(howMany);
                }
                
                else
                {}
                
                break;
            }
        }
    }
    
    //TRY TO MOVE IN THAT DIRECTION
    GraphObject::Direction move = getDirection();
    switch (move)
    {
        case up:
            if (getWorld()->canActorMoveTo(this, getX(), getY()+1))
            {
                moveTo(getX(), getY()+1);
                decOtherTicks();
            }
            else
            {
                setNumSquaresToMove(0);
                decOtherTicks();
            }
            break;
            
        case down:
            if (getWorld()->canActorMoveTo(this, getX(), getY()-1))
            {
                moveTo(getX(), getY()-1);
                decOtherTicks();
            }
            else
            {
                setNumSquaresToMove(0);
                decOtherTicks();
            }
            break;
        case left:
            if (getWorld()->canActorMoveTo(this, getX()-1, getY()))
            {
                moveTo(getX()-1, getY());
                decOtherTicks();
            }
            
            else
            {
                setNumSquaresToMove(0);
                decOtherTicks();
            }
            break;
            
        case right:
            if (getWorld()->canActorMoveTo(this,getX()+1, getY()))
            {
                moveTo(getX()+1, getY());
                decOtherTicks();
            }
            else
            {
                setNumSquaresToMove(0);
                decOtherTicks();
            }
    }
    
}








//////////////////////////////////////////////////////
///////                                    ///////////
///////              SQUIRT                ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

Squirt::Squirt(StudentWorld* world, int startX, int startY, Direction startDir): Actor(world, IID_WATER_SPURT, startX, startY, startDir, 1, 1, 0)
{
    setVisible(true);
    m_travelDistance = 4;
}

Squirt::~Squirt()
{
    setVisible(false);
}

void Squirt::doSomething()
{
    
    /*IF THE SQUIRT IS IN RADIUS 3 OF A PROTESTOR, CAUSE 3 POINTS OF ANNOYANCE
     TO THESE PROTESTORS, THEN SET ITSELF TO DEAD*/
    
    int howMany = getWorld()->annoyAllNearbyProtestors(this, 2, 3);
    if (howMany > 0)
    {
        setDead();
        return;
    }

    
    
    /*IF THE SQUIRT HAS TRAVELED THROUGH ITS FULL DISTANCE, SET STATE TO DEAD*/
    
    if (m_travelDistance == 0)
    {
        setDead();
        return;
    }
    
    
    /*CHECK TO SEE IF SQUIRT CAN MOVE, IF THERE IS A DIRT OR BOULDER IN THE WAY, SET TO DEAD*/
    
    /* IF IT CAN MOVE, MOVE IN THE SPECIFIED DIRECTION */
    
    
    Direction dir = getDirection();
    switch (dir)
    {
        case up:
            if (getWorld()->canActorMoveTo(this, getX(), getY()+1))
            {
                moveTo(getX(), getY() + 1);
                m_travelDistance--;
                //return;
            }
            else
            {
                setDead();
                return;
            }
            break;
        case down:
            if (getWorld()->canActorMoveTo(this, getX(), getY()-1))
            {
                moveTo(getX(), getY() -1);
                m_travelDistance--;
                //return;
            }
            else
            {
                setDead();
                return;
            }
            break;
        case left:
            if (getWorld()->canActorMoveTo(this, getX() - 1, getY()))
            {
                moveTo(getX() - 1, getY());
                m_travelDistance--;
                //return;
            }
            else
            {
                setDead();
                return;
            }
            break;
        case right:
            if (getWorld()->canActorMoveTo(this, getX() + 1, getY()))
            {
                moveTo(getX() + 1, getY());
                m_travelDistance--;
                //return;
            }
            else
            {
                setDead();
                return;
            }
            break;
        default:
            return;
    }
    
    
    
    
   
    

}





//////////////////////////////////////////////////////
///////                                    ///////////
///////              GOODIE                ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

//constructor
Goodie::Goodie(StudentWorld* world, int imageID, int startX, int startY,int soundToPlay, bool activateOnPlayer, bool activateOnProtester, bool initallyActive): Actor(world, imageID, startX, startY, right /*IS THIS RIGHT?*/, 1.0, 2.0, 0)
{
    setVisible(false);
    m_activateFrackMen = activateOnPlayer;
    m_activateProtestor = activateOnProtester;
    m_active = initallyActive;
}

Goodie::~Goodie()
{
    setVisible(false);
}

//Return if frackman can pick him up
bool Goodie::FrackManCanPickup()
{
    return m_activateFrackMen;
}


//////////////////////////////////////////////////////
///////                                    ///////////
///////                 OIL                ///////////
///////                                    ///////////
//////////////////////////////////////////////////////


Oil::Oil(StudentWorld* world, int startX, int startY): Goodie(world, IID_BARREL, startX, startY, SOUND_FOUND_OIL, true, false, false)
{
    setVisible(false);
}

Oil::~Oil()
{
    setVisible(false);
}

void Oil::doSomething()
{
    //IF IT ISNT ALIVE, RETURN//
    if (!isAlive())
        return;
    
    //IF NOT VISIBLE AND FRACKMAN WITHIN RADIUS 4, MAKE VISIBLE//
    if (!isActive() && getWorld()->withinRadius(4.0, getX(), getY(), getWorld()->getFrackX(), getWorld()->getFrackY()))
    {
        setActive();
        setVisible(true);
        return;
    }
    
    //IF WITHIN RADIUS 3 OF FRACKMAN, MUST ACTIVATE//
    if (getWorld()->withinRadius(3.0, getX(), getY(), getWorld()->getFrackX(), getWorld()->getFrackY()))
    {
        setDead();
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
        
        //DECREMENT NUMBER OF OBJECTIVES REMAINING
        getWorld()->removeOilObjective();
        
    }
    
    
    
}


//////////////////////////////////////////////////////
///////                                    ///////////
///////              GOLD                  ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

//constructor
Gold::Gold(StudentWorld* world, int startX, int startY, bool protestorCanPickup): Goodie(world, IID_GOLD, startX, startY, SOUND_PROTESTER_FOUND_GOLD, !protestorCanPickup, protestorCanPickup, protestorCanPickup)
{
    if (protestorCanPickup)
    {
        setVisible(true);
        setTicks();
    }
    else
        setVisible(false);
}

void Gold::setTicks()
{
    int level = getWorld()->getLevel();
    TicksLeft = max(100, 300-(10*level));
}

//destructor
Gold::~Gold()
{
    setVisible(false);
}

void Gold::doSomething()
{
   //CHECK IF ALIVE, IF NOT, RETURN//
    if (!isAlive())
        return;
    
    //If it is not visible and frackman is near, set visible//
    if (!isActive() && getWorld()->withinRadius(4.0, getX(), getY(), getWorld()->getFrackX(), getWorld()->getFrackY()))
    {
        setVisible(true);
        setActive();
        return;
    }
    
    //IF IT CAN BE PICKED UP BY FRACKMAN && WITHIN RADIUS OF 3, GET PICKUP UP//
    if (FrackManCanPickup() && getWorld()->withinRadius(3.0, getX(), getY(), getWorld()->getFrackX(), getWorld()->getFrackY()))
        {
            setDead();
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->giveFrackGold();
            getWorld()->increaseScore(10);
            return;
        }
    
    Protestor* pp = getWorld()->findNearbyProtestor(this, 3);
    //IF IT CAN BE PICKED UP BY THE PROTESTOR, BRIBE HIM//
    if (!FrackManCanPickup() && (pp != nullptr))
    {
        pp->bribeProtestor();
        setDead();
    }
    
    //IF IT HAS NO TICKS LEFT, DESTROY
    if (!FrackManCanPickup() && TicksLeft == 0)
    {
        setDead();
    }
    
    TicksLeft--;
    
    
}






//////////////////////////////////////////////////////
///////                                    ///////////
///////            SONAR KIT               ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

SonarKit::SonarKit(StudentWorld* world, int startX, int startY): Goodie(world, IID_SONAR, startX, startY, SOUND_GOT_GOODIE, true, false, true)
{
    setVisible(true);
    setTicks();
}

void SonarKit::setTicks()
{
    int level = getWorld()->getLevel();
    TicksLeft = max(100, 300-(10*level));
}

SonarKit::~SonarKit()
{
    setVisible(false);
}

void SonarKit::doSomething()
{
    //IF NOT ALIVE, RETURN//
    if (!isAlive())
        return;
    
    //IF WITHIN RADIUS 3 OF FRACKMAN, GIVE HIM A KIT//
    if (getWorld()->withinRadius(3.0, getX(), getY(), getWorld()->getFrackX(), getWorld()->getFrackY()))
    {
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->giveFrackSonar();
        getWorld()->increaseScore(75);
        return;
    }
    
    if (TicksLeft == 0)
    {
        setDead();
        return;
    }
    
    TicksLeft--;
    
}










//////////////////////////////////////////////////////
///////                                    ///////////
///////              WATER REFILL          ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

WaterRefill::WaterRefill(StudentWorld* world, int startX, int startY): Goodie(world, IID_WATER_POOL, startX, startY, SOUND_GOT_GOODIE, true, false, true)
{
    setVisible(true);
    setTicks();
}

void WaterRefill::setTicks()
{
    int level = getWorld()->getLevel();
    TicksLeft = max(100, 300-(10*level));
}

WaterRefill::~WaterRefill()
{
    setVisible(false);
}

void WaterRefill::doSomething()
{
    //IF NOT ALIVE, RETURN//
    if (!isAlive())
        return;
    
    //IF WITHIN RADIUS 3, GIVE FRACKMAN 5 SQUIRTS//
    if (getWorld()->withinRadius(3.0, getX(), getY(), getWorld()->getFrackX(), getWorld()->getFrackY()))
    {
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->giveFrackSquirts();
        getWorld()->increaseScore(100);
        return;
    }
    
    //CHECK TO SEE IF IT CAN MOVE ANY MORE//
    if (TicksLeft == 0)
    {
        setDead();
        return;
    }
    
    TicksLeft--;
}




