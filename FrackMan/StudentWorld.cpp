
#include "StudentWorld.h"
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <sstream>
#include <iomanip>



using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


StudentWorld::StudentWorld(string assetDir):GameWorld(assetDir)
{
    m_frackman = nullptr;
    srand(time(NULL));
    m_oilLeft = 0;
}


StudentWorld::~StudentWorld()
{
    //DELETE ALL DIRT OBJECTS//
    
    list<Dirt*>::iterator p = m_dirt.begin();
    while (p != m_dirt.end())
    {
        list<Dirt*>::iterator q = p;
        p++;
        Dirt* dp = *q; //Make the dirt pointer easily accessable
        delete dp;
        m_dirt.erase(q);
        
    }
    
    
    
    //DELETE ALL GAME OBJECTS//
    list<Actor*>::iterator a = m_gameobjects.begin();
    while (a != m_gameobjects.end())
    {
        list<Actor*>::iterator b = a;
        a++;
        Actor* actorValue = *b; //Make the actor pointer easily accessable
        delete actorValue;
        m_gameobjects.erase(b);
    }
}




//////////////////////////////////////////////////////
///////                                    ///////////
///////              findActor()            ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

bool StudentWorld::findActor(Actor* ap, int x, int y)
{
    //SEE IF DIRT EXISTS AT THIS POINT X,Y//
    
    if (ap->getX() == x && ap->getY() == y)
        return true;
    return false;
}



//////////////////////////////////////////////////////
///////                                    ///////////
///////           dig()                    ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

bool StudentWorld::dig(int x, int y)
{
    bool hasRemoved = false;
    
    list<Dirt*>::iterator p = m_dirt.begin();
    
    while (p != m_dirt.end())
    {
        Dirt* dp = *p;
        int dirtX = dp->getX();
        int dirtY = dp->getY();
        if (dp->getWorld()->between(dirtX, x, x+3) && dp->getWorld()->between(dirtY, y, y + 3))
            {
                hasRemoved = true;
                list<Dirt*>::iterator q = p;
                p++;
                delete dp;
                m_dirt.erase(q);
            }
        p++;
    }
    return hasRemoved;
}




//////////////////////////////////////////////////////
///////                                    ///////////
///////           removeDirt()             ///////////
///////                                    ///////////
//////////////////////////////////////////////////////



void StudentWorld::removeDirt(int x, int y)
{
    
    list<Dirt*>::iterator p = m_dirt.begin();
    
    while (p != m_dirt.end())
    {
        Dirt* dp = *p;
        int dirtX = dp->getX();
        int dirtY = dp->getY();
        if (dp->getWorld()->between(dirtX, x, x+3) && dp->getWorld()->between(dirtY, y, y + 3))
        {
            list<Dirt*>::iterator q = p;
            p++;
            delete dp;
            m_dirt.erase(q);
        }
        p++;
    }
}


//////////////////////////////////////////////////////
///////                                    ///////////
///////        dirtUnderBoulder()          ///////////
///////                                    ///////////
//////////////////////////////////////////////////////


bool StudentWorld::dirtUnderBoulder(int x, int y)
{
    //PASSED IN POSITION OF BOULDER, NEED TO CHECK 4 SPOTS UNDER//
    
    
        list<Dirt*>::iterator p = m_dirt.begin();
        while (p != m_dirt.end())
        {
            Dirt* dp = *p;
            int dirtX = dp->getX();
            int dirtY = dp->getY();
            if (dp->getWorld()->between(dirtX, x, x+3) && dirtY == y-1)
                return true;
            p++;
        }
    
    return false;
    
    
}


//////////////////////////////////////////////////////
///////                                    ///////////
///////           inMineShaft()            ///////////
///////                                    ///////////
//////////////////////////////////////////////////////


bool StudentWorld::inMineshaft(int x)
{
    if (x >= 27 && x < 34)
        return true;
    return false;
}



//////////////////////////////////////////////////////
///////                                    ///////////
///////               random()             ///////////
///////                                    ///////////
//////////////////////////////////////////////////////



int StudentWorld::random(int begin, int end)
{
    return rand() % (end-begin+1)+begin;
}


//////////////////////////////////////////////////////
///////                                    ///////////
///////              between()             ///////////
///////                                    ///////////
//////////////////////////////////////////////////////


bool StudentWorld::between(int check, int varbegin, int varend) const
{
    if (check >= varbegin && check <= varend)
        return true;
    return false;
}


//////////////////////////////////////////////////////
///////                                    ///////////
///////           canActorMoveTo()         ///////////
///////                                    ///////////
//////////////////////////////////////////////////////



bool StudentWorld::canActorMoveTo(Actor* ap, int x, int y) const
{
    //CHECK FOR BOUNDARIES//
    if (x < 0 || x > 60)
        return false;
    if (y < 0 || y > 60)
        return false;
    
    
    //CHECK FOR DIRT//
    if (!ap->canDigThroughDirt())
    {
        list<Dirt*>::const_iterator p = m_dirt.begin();
        while (p != m_dirt.end())
        {
            Dirt* dp = *p;
            for (int i = x; i < x+4; i++)
            {
                for (int j = y; j < y+4; j++)
                {
                    
                    if (i == dp->getX() && j == dp->getY())
                        return false;
                }
            }
            
            p++;
        }
    }
    //CHECK FOR OTHER BOULDERS//
    
    list<Actor*>::const_iterator q = m_gameobjects.begin();
    while (q != m_gameobjects.end())
    {
        Actor* actorpointer = *q;
        if (!(actorpointer->canActorsPassThroughMe()) && actorpointer != ap)
        {
            
            for (int i = x; i < x+4; i++)
            {
                for (int j = y; j < y+4; j++)
                {
            
            if (between(i, actorpointer->getX(), actorpointer->getX()+3) && between(j, actorpointer->getY(), actorpointer->getY()+3))
                return false;
                }
            }
        }
        q++;
    }
    
    return true;
}



//////////////////////////////////////////////////////
///////                                    ///////////
///////           withinRadius()           ///////////
///////                                    ///////////
//////////////////////////////////////////////////////


bool StudentWorld::withinRadius(double rad, double x1, double y1, double x2, double y2) const
{
    double checker = sqrt(pow( (x2-x1),2) + pow( (y2-y1), 2) );
    if (checker <= rad)
        return true;
    else
        return false;
}


//////////////////////////////////////////////////////
///////                                    ///////////
///////           initBoulders()           ///////////
///////                                    ///////////
//////////////////////////////////////////////////////


void StudentWorld::initBoulders(int level) //STILL HAS PROBLEM WITH RNG!!!!!!!!!
{
    //INITIALIZE THE BOULDERS//
    
    //ERROR!!! THIS FUNCTION DOES NOT WORK UNLESS BREAKPOINTS ARE SET//
    int B =  min((level/2) + 2, 6);
    while (B != 0)
    {
        //GET AN X AND Y TO PROPOSE THE BOULDERS//
        int x = random(0, 60);
        int y = random(20,55);
        
        //CHECK MINESHAFT// CHECK THIS
        
        if (inMineshaft(x))
            continue;
        
        //CHECK PROXIMITY TO OTHER OBJECTS//
        list<Actor*>::iterator prox = m_gameobjects.begin();
        bool allOutside = true;
        while (prox != m_gameobjects.end())
        {
            Actor* helpMe = *prox;
            if (withinRadius(6, x, y, helpMe->getX(), helpMe->getY()))
            {
                allOutside = false;
                break;
            }
            
            prox++;
            
        }
        
        if (allOutside == false)
            continue;
        
        
        
        //IF VALID, CREATE THAT NEW OBJECT, REMOVE DIRT AND DECREASE B//
        
        m_gameobjects.push_back(new Boulder(this, x, y));
        removeDirt(x, y);
        B--;
        
        
    }
}







//////////////////////////////////////////////////////
///////                                    ///////////
///////        annoyAllNearbyActors()      ///////////
///////                                    ///////////
//////////////////////////////////////////////////////


//THIS FUNCTION IS A DRAFT
int StudentWorld::annoyAllNearbyActors(Actor* annoyer, int points, int radius)
{
    int amtAnnoyed = 0;
    
    //FRACKMAN//
    
    if (withinRadius(radius, annoyer->getX(), annoyer->getY(), m_frackman->getX(), m_frackman->getY()))
    {
        if (m_frackman->annoy(points))
            amtAnnoyed++;
    }
    
    //PROTESTORS
    
    list<Protestor*>::iterator protestIter = m_protestors.begin();
    while (protestIter != m_protestors.end())
    {
        Protestor* pp = *protestIter;
        if (withinRadius(radius, annoyer->getX(), annoyer->getY(), pp->getX(), pp->getY()) && pp->canAnnoy())
        {
            amtAnnoyed++;
            if (pp->annoy(points))
            {
                pp->setLeaving();
                if (points == 2)
                    increaseScore(100);
                else if(points == 100)
                    increaseScore(500);
            }
        }
        
        protestIter++;
    }
    
    return amtAnnoyed;
}


//////////////////////////////////////////////////////
///////                                    ///////////
///////      annoyAllNearbyProtestors()    ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

int StudentWorld::annoyAllNearbyProtestors(Actor* annoyer, int points, int radius)
{
    int amtAnnoyed = 0;
    
    //PROTESTORS
    
    list<Protestor*>::iterator protestIter = m_protestors.begin();
    while (protestIter != m_protestors.end())
    {
        Protestor* pp = *protestIter;
        if (withinRadius(radius, annoyer->getX(), annoyer->getY(), pp->getX(), pp->getY()) && pp->canAnnoy())
        {
            amtAnnoyed++;
            if (pp->annoy(points))
            {
                pp->setLeaving();
                if (points == 2)
                    increaseScore(100);
                else if(points == 100)
                    increaseScore(500);
            }
            
        }
        
        protestIter++;
    }
    
    return amtAnnoyed;
}




//////////////////////////////////////////////////////
///////                                    ///////////
///////            addSquirt()             ///////////
///////                                    ///////////
//////////////////////////////////////////////////////


void StudentWorld::addSquirt(int x, int y, Actor::Direction dir)
{
    //IF THERE IS A DIRT AT X,Y DONT ADD SQUIRT
    list<Dirt*>::iterator dIter = m_dirt.begin();
    while (dIter != m_dirt.end())
    {
        Dirt* poop = *dIter;
        if (findActor(poop, x, y))
            return;
        dIter++;
    }
    
    //OTHERWISE, ADD A SQUIRT
    m_gameobjects.push_back(new Squirt(this, x, y, dir));
    
}

//////////////////////////////////////////////////////
///////                                    ///////////
///////             addGold()              ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

void StudentWorld::addGold(int x, int y)
{
    m_gameobjects.push_back(new Gold(this,x,y,true));
}



//////////////////////////////////////////////////////
///////                                    ///////////
///////              getFrackX()           ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

int StudentWorld::getFrackX() const
{
    return m_frackman->getX();
}



//////////////////////////////////////////////////////
///////                                    ///////////
///////              getFrackY()           ///////////
///////                                    ///////////
//////////////////////////////////////////////////////



int StudentWorld::getFrackY() const
{
    return m_frackman->getY();
}



//////////////////////////////////////////////////////
///////                                    ///////////
///////         removeOilObjective()       ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

void StudentWorld::removeOilObjective()
{
    m_oilLeft--;
}

//////////////////////////////////////////////////////
///////                                    ///////////
///////              oilLeft()             ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

int StudentWorld::oilLeft()
{
    return m_oilLeft;
}



//////////////////////////////////////////////////////
///////                                    ///////////
///////        initOilNumber()             ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

void StudentWorld::initOilNumber(int level)
{
    m_oilLeft = min(2+level, 20);
}

//////////////////////////////////////////////////////
///////                                    ///////////
///////        giveFrackGold()             ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

void StudentWorld::giveFrackGold()
{
    m_frackman->giveGold();
}
//////////////////////////////////////////////////////
///////                                    ///////////
///////                initOil()           ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

void StudentWorld::initOil()
{
    int L = m_oilLeft;
    
    while (L != 0)
    {
        //GET A RANDOM X AND Y
        int x = random(0,60);
        int y = random(20, 56);
        
        //CHECK MINESHAFT
        
        if (inMineshaft(x))
            continue;
        
        //CHECK PROX TO OTHER OBJECTS
        list<Actor*>::iterator prox = m_gameobjects.begin();
        bool allOutside = true;
        while (prox != m_gameobjects.end())
        {
            Actor* helpMe = *prox;
            if (withinRadius(6, x, y, helpMe->getX(), helpMe->getY()))
            {
                allOutside = false;
                break;
            }
            
            prox++;
            
        }
        
        if (allOutside == false)
            continue;
        
        
        //IF VALID, CREATE THAT NEW OBJECT AND DECREASE L//
        m_gameobjects.push_back(new Oil(this,x,y));
        L--;
        
    }
    
}



//////////////////////////////////////////////////////
///////                                    ///////////
///////          giveFrackSonar()          ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

void StudentWorld::giveFrackSonar()
{
    m_frackman->giveSonar();
}


//////////////////////////////////////////////////////
///////                                    ///////////
///////       giveFrackSquirts()           ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

void StudentWorld::giveFrackSquirts()
{
    m_frackman->giveSquirts();
}



//////////////////////////////////////////////////////
///////                                    ///////////
///////        findNearbyProtestor()       ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

Protestor* StudentWorld::findNearbyProtestor(Actor* a, int radius) const
{
    list<Protestor*>::const_iterator fml= m_protestors.begin();
    while (fml!= m_protestors.end())
    {
        Protestor* p = *fml;
        if (withinRadius(3.0, a->getX(), a->getY(), p->getX(), p->getY()) && p->canAnnoy())
        {
            return p;
        }
        
        fml++;
    }
    
    return nullptr;
}

//////////////////////////////////////////////////////
///////                                    ///////////
///////              initGold()            ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

void StudentWorld::initGold(int level)
{
    int G = max((5-level)/2,2);
    
    while (G != 0)
    {
        //GET A RANDOM X AND Y
        int x = random(0,60);
        int y = random(20,56);
        
        //CHECK MINESHAFT
        
        if (inMineshaft(x))
            continue;
        
        //CHECK PROX TO OTHER OBJECTS
        list<Actor*>::iterator prox = m_gameobjects.begin();
        bool allOutside = true;
        while (prox != m_gameobjects.end())
        {
            Actor* helpMe = *prox;
            if (withinRadius(6, x, y, helpMe->getX(), helpMe->getY()))
            {
                allOutside = false;
                break;
            }
            
            prox++;
            
        }
        
        if (allOutside == false)
            continue;
        
        
        //IF VALID, CREATE OBJECTS AND DECREASE G
        m_gameobjects.push_back(new Gold(this,x,y,false));
        G--;
        
    }
        
}


//////////////////////////////////////////////////////
///////                                    ///////////
///////            clearDirt()             ///////////
///////                                    ///////////
//////////////////////////////////////////////////////


bool StudentWorld::clearDirt(int x, int y) const
{
    list<Dirt*>::const_iterator p = m_dirt.begin();
    while (p != m_dirt.end())
    {
        Dirt* dp = *p;
        for (int i = x; i < x+4; i++)
        {
            for (int j = y; j < y+4; j++)
            {
                
                if (i == dp->getX() && j == dp->getY())
                    return false;
            }
        }
        
        p++;
    }
    
    return true;
}

//////////////////////////////////////////////////////
///////                                    ///////////
///////            addGoodies()            ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

void StudentWorld::addGoodies(int level)
{
    int G = (level*25 + 300);
    int checker = random(1,G);
    
    if (checker == 69)  //3 DANK 5 ME
    {
        
        int poopy = random(1, 5);
        if (poopy == 1)
        {
            m_gameobjects.push_back(new SonarKit(this,0,60));
        }
        else
        {
            bool complete = false;
            while (complete != true)
            {
                int x = random(0,60);
                int y = random(0,60);
                if (clearDirt(x,y))
                {
                    if (canActorMoveTo(m_frackman, x, y))
                    {
                    m_gameobjects.push_back(new WaterRefill(this,x,y));
                    complete = true;
                    }
                }
            }
        }
    }
}



//////////////////////////////////////////////////////
///////                                    ///////////
///////          updateGameText()          ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

void StudentWorld::updateGameText()
{
    int score = getScore();
    int level = getLevel();
    int lives = getLives();
    int health = (m_frackman->getHP() * 10);
    int squirts = m_frackman->squirtsLeft();
    int sonars = m_frackman->sonarsLeft();
    int gold = m_frackman->goldLeft();
    int oilLeft = m_oilLeft;
    
    ostringstream oss;
    oss.fill('0');
    oss << "Scr: " << setw(6) << score << "  ";
    oss.fill(' ');
    oss << "Lvl: "  << setw(2) << level << "  ";
    oss  << "Lives: "  << setw(1) << lives << "  ";
    oss  << "Hlth: " << setw(3) << health << "%  ";
    oss  << "Wtr: " << setw(2) << squirts << "  ";
    oss  << "Gld: " << setw(2) << gold << "  ";
    oss  << "Sonar: "  << setw(2) << sonars << "  ";
    oss  << "Oil Left: " << setw(2) << oilLeft;
    
    string s = oss.str();
    
    setGameStatText(s);
}



//////////////////////////////////////////////////////
///////                                    ///////////
///////          useSonarKit()             ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

void StudentWorld::useSonarKit()
{
    list<Actor*>::iterator q = m_gameobjects.begin();
    while (q != m_gameobjects.end())
    {
        Actor* poop = *q;
        if (withinRadius(12, getFrackX(), getFrackY(), poop->getX(), poop->getY()))
            poop->setVisible(true);
        q++;
    }
}

//////////////////////////////////////////////////////
///////                                    ///////////
///////    facingTowardsFrackman()         ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

bool StudentWorld::facingTowardsFrackman(Actor* a)
{
    GraphObject::Direction dir = a->getDirection();
    switch (dir)
    {
        case Actor::up:
            if (a->getY() <= getFrackY())
                return true;
            break;
        case Actor::down:
            if (a->getY() >= getFrackY())
                return true;
            break;
        case Actor::left:
            if (a->getX() >= getFrackX())
                return true;
            break;
        case Actor::right:
            if (a->getX() <= getFrackX())
                return true;
        default:
            return false;
    }
    
    return false;
}


//////////////////////////////////////////////////////
///////                                    ///////////
///////    lineofSighttoFrackman()         ///////////
///////                                    ///////////
//////////////////////////////////////////////////////


GraphObject::Direction StudentWorld::lineofSighttoFrackman(Actor* a) const
{
    if (a->getX() == getFrackX())
    {
        if (a->getY() <= getFrackY())
        {
            for (int i = a->getY(); i <= getFrackY(); i++)
            {
                
                if (!(canActorMoveTo(a,a->getX(), i)))
                    return GraphObject::none;
            }
            
            return GraphObject::up;
        }
        
        else if (a->getY() >= getFrackY())
        {
            for (int i = a->getY(); i >= getFrackY(); i--)
            {
                if (!canActorMoveTo(a,a->getX(), i))
                    return GraphObject::none;
            }
            
            return GraphObject::down;
        }
    }
    
    if (a->getY() == getFrackY())
    {
        if (a->getX() <= getFrackX())
        {
            for (int i = a->getX(); i <= getFrackX(); i++)
            {
                if (!canActorMoveTo(a,i, a->getY()))
                    return GraphObject::none;
            }
           
            return GraphObject::right;
        }
        
        else if(a->getX() >= getFrackX())
        {
            for (int i = a->getX(); i >= getFrackX(); i--)
            {
                if (!canActorMoveTo(a,i, a->getY()))
                    return GraphObject::none;
            }
            
            return GraphObject::left;
        }
    }
    
    return GraphObject::none;
}

//////////////////////////////////////////////////////
///////                                    ///////////
///////         getFrackman()               ///////////
///////                                    ///////////
//////////////////////////////////////////////////////

Actor* StudentWorld::getFrackman()
{
    return m_frackman;
}






//////////////////////////////////////////////////////
///////                                    ///////////
///////                INIT()              ///////////
///////                                    ///////////
//////////////////////////////////////////////////////



int StudentWorld::init()
{
    //GET THE LEVEL//
    int level = getLevel();
    
    //SET THE AMOUNT OF OIL//
    initOilNumber(level);
    
    //Set Max Protestors
    double minimum = min(15.0,2+level*1.5);
    int min = minimum;
    maxProtestors = min;
    
    //Set max number of ticks until a new protestor can be added
    int maximum = max(25, 200-level);
    ticksUntilNewProtestor = maximum;
    
    
    
    //DIRT LEFT OF MINESHAFT//
    
    for (int i = 0; i <= 59; i++)
        for (int j = 0; j < 30; j++)
            m_dirt.push_back(new Dirt(this,j,i));
    
    //DIRT RIGHT OF MINESHAFT//
    for (int i = 0; i <=  59; i++)
        for (int j = 34; j < 64; j++)
            m_dirt.push_back(new Dirt(this,j,i)); //Dirt right of mineshaft
    
    //DIRT UNDER MINESHAFT//
    for (int i = 0; i < 4; i ++)
        for (int j = 30; j <= 33; j++)
            m_dirt.push_back(new Dirt(this,j,i)); //Dirt under shaft
    
    //INITIALIZE FRACKMAN//
    m_frackman = new FrackMan(this);
    
    //INITIALIZE THE BOULDERS//
    initBoulders(level);
    
    //SET THE OIL//
    initOil();
    
    //SET THE GOLD//
    initGold(level);
    
    //SET THE PROTESTOR//
    int prob = std::__1::min(90, 10*level + 30);
    int chance = random(1,100);
    if (chance <= prob)
    {
        m_protestors.push_back(new HardcoreProtestor(this,60,60));
    }
    else
        m_protestors.push_back(new NormalProtestor(this,60,60));
    
    updateGameText();

    
    
    return GWSTATUS_CONTINUE_GAME; //Return to the game after initializing
}


//////////////////////////////////////////////////////
///////                                    ///////////
///////               MOVE()               ///////////
///////                                    ///////////
//////////////////////////////////////////////////////


int StudentWorld::move()
{
    int level = getLevel();
    
    //UPDATE GAME TEXT//
    updateGameText();
    
    //TELL FRACKMAN TO DO SOMETHING//
    
    m_frackman->doSomething();
    
    //TELL ALL GAME OBJECTS TO DO SOMETHING//
    list<Actor*>::iterator p = m_gameobjects.begin();
    while (p != m_gameobjects.end())
    {
        Actor* ap = *p;
        ap->doSomething();
        p++;
    }
    
    //TELL PROTESTORS TO DO SOMETHING//
    list<Protestor*>::iterator prot = m_protestors.begin();
    while (prot != m_protestors.end())
    {
        Protestor* poopy = *prot;
        poopy->doSomething();
        prot++;
    }
    
    //ADD GOODIES
    addGoodies(level);
    
    //Add a new protestor if possible
    if (ticksUntilNewProtestor == 0 && m_protestors.size() < maxProtestors)
    {
        
        //SET THE PROTESTOR//
        int prob = std::__1::min(90, 10*level + 30);
        int chance = random(1,100);
        if (chance <= prob)
        {
            m_protestors.push_back(new HardcoreProtestor(this,60,60));
            int maximum = max(25, 200-level);
            ticksUntilNewProtestor = maximum;
            
        }
        else
        {
            m_protestors.push_back(new NormalProtestor(this,60,60));
            int maximum = max(25, 200-level);
            ticksUntilNewProtestor = maximum;
        }
    }
    
    
    if (ticksUntilNewProtestor != 0)
        ticksUntilNewProtestor--;
    
    
    
    //REMOVE ALL DEAD OBJECTS FROM THE GAME//
    
        //gameobjects//
    list<Actor*>::iterator aIter = m_gameobjects.begin();
    while (aIter != m_gameobjects.end())
    {
        Actor* check = *aIter;
        if (!(check->isAlive()) )
        {
            list<Actor*>::iterator q = aIter;
            aIter++;
            delete check;
            m_gameobjects.erase(q);
        }
        aIter++;
    }
    
        //protestors//
    
    list<Protestor*>::iterator protest = m_protestors.begin();
    while (protest != m_protestors.end())
    {
        Protestor* checkProtestor = *protest;
        if (!(checkProtestor->isAlive()))
        {
            list<Protestor*>::iterator qq = protest;
            protest++;
            delete checkProtestor;
            m_protestors.erase(qq);
        }
        
        protest++;
    }
    
    //RETURN PROPER RESULT IF PLAYER DIED
    
    if (!(m_frackman->isAlive()) )
    {
        decLives();
        playSound(SOUND_PLAYER_GIVE_UP);
        return GWSTATUS_PLAYER_DIED;
    }
    
    //IF ALL BARRELS GONE, RETURN FINISHED LEVEL
    if (oilLeft() == 0)
    {
        playSound(SOUND_FINISHED_LEVEL);
        
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}




//////////////////////////////////////////////////////
///////                                    ///////////
///////            CLEANUP()               ///////////
///////                                    ///////////
//////////////////////////////////////////////////////



void StudentWorld::cleanUp()
{
    //DELETE ALL DIRT OBJECTS//
    
        list<Dirt*>::iterator p = m_dirt.begin();
        while (p != m_dirt.end())
        {
            list<Dirt*>::iterator q = p;
            p++;
            Dirt* dp = *q; //Make the dirt pointer easily accessable
            delete dp;
            m_dirt.erase(q);
        
        }

    //DELETE THE FRACKMAN//
    delete m_frackman;
    m_frackman = nullptr;
    
    //DELETE ALL GAMEOBJECTS//
    list<Actor*>::iterator a = m_gameobjects.begin();
    while (a != m_gameobjects.end())
    {
        list<Actor*>::iterator b = a;
        a++;
        Actor* actorValue = *b; //Make the actor pointer easily accessable
        delete actorValue;
        m_gameobjects.erase(b);
    }
    
    //DELETE ALL PROTESTORS//
    list<Protestor*>::iterator poop = m_protestors.begin();
    while (poop != m_protestors.end())
    {
        list<Protestor*>::iterator pee = poop;
        poop++;
        Protestor* poopValue = *pee;
        delete poopValue;
        m_protestors.erase(pee);
        
    }
}



            



        
