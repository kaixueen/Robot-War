#ifndef ROBOT_H
#define ROBOT_H
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class Battlefield; // forward declaration

// Robot class
class Robot
{
private:
    string robotName;
    string robotType;
    char robotSymbol;
    int robotPositionX, robotPositionY;
    int remainingLives;
    Robot *robotTerminated[3];
    int numOfRobotTerminated;
    int currentNumOfRobotTerminated;
    bool upgradePermission;

public:
    Robot();                                         // Default constructor
    Robot(string t, string n, int x, int y, char s); // Parameterized constructor
    Robot(const Robot &r);                           // Copy constructor
    Robot(Robot &&r) noexcept;                       // move constructor
    Robot &operator=(const Robot &right);            // Assignment operator overloading
    virtual ~Robot();                                // Destructor

    string getName() const { return robotName; } // Return robot name
    string getType() const { return robotType; } // Return robot type
    char getSymbol() const { return robotSymbol; }
    void setName(string n) { robotName = n; }
    void setType(string t) { robotType = t; }
    void setSymbol(char r) { robotSymbol = r; }

    int getX() const { return robotPositionX; } // Return robot x coordinate
    int getY() const { return robotPositionY; } // Return robot y coordinate
    void setX(int x) { robotPositionX = x; }    // Set robot x coordinate
    void setY(int y) { robotPositionY = y; }    // Set robot y coordinate

    bool stillGotLive() const { return remainingLives > 0; } // Remaining lives > 0?
    int getRemainingLives() const { return remainingLives; } // Return remaining lives
    void setRemainingLives(int l) { remainingLives = l; }    // Set remaining lives

    void setRobotTerminated(Robot &r);                                     // store robot terminated
    Robot &getRobotTerminated(int n) const { return *robotTerminated[n]; } // return robot terminated
    int getNumOfRobotTerminated() const { return numOfRobotTerminated; }   // return number of robot terminated
    void resetRobotTerminated();                                           // reset robot terminated to nullptr
    void resetNumOfRobotTerminated() { numOfRobotTerminated = 0; }
    void resetCurrentNumOfRobotTerminated() { currentNumOfRobotTerminated = 0; }
    int getCurrentNumOfRobotTerminated() const { return currentNumOfRobotTerminated; }

    virtual void takeTurn(Battlefield &bt, ofstream &outfile) = 0;  // Pure virtual function
    bool getUpgradePermission() const { return upgradePermission; } // Valid for upgrade?
    void setUpgradePermission(bool p) { upgradePermission = p; }    // Set upgradePermission
};

// RobotNode class (for linked list and queue implementation)
class RobotNode
{
private:
    Robot *robot;
    RobotNode *next;

public:
    RobotNode() : robot(nullptr), next(nullptr) {}                         // default constructor
    RobotNode(Robot *rb) : robot(rb), next(nullptr) {}                     // parameterized constructor
    RobotNode(Robot *rb, RobotNode *nextPtr) : robot(rb), next(nextPtr) {} // parameterized constructor

    RobotNode(const RobotNode &rn) : robot(rn.robot), next(rn.next) {} // copy constructor
    RobotNode(RobotNode &&rn);                                         // move constructor
    RobotNode &operator=(const RobotNode &rn);                         // copy assignment operator

    ~RobotNode() {}

    void setRobot(Robot *rb) { robot = rb; }
    void setNext(RobotNode *nextPtr) { next = nextPtr; }
    Robot *getRobot() const { return robot; }
    RobotNode *getNext() const { return next; }
};

// RobotList (linked list)
class RobotList
{
private:
    RobotNode *headPtr;
    int robotCount;

public:
    RobotList() : headPtr(nullptr), robotCount(0) {} // default constructor
    RobotList(const RobotList &rl);                  // copy constructor
    RobotList(RobotList &&rl);                       // move constructor
    RobotList &operator=(const RobotList &rl);       // copy assignment operator

    ~RobotList(); // destructor

    bool isListEmpty() const { return robotCount == 0; }
    int getListLength() const { return robotCount; }
    RobotNode *getNodeAt(int position);
    void appendRobot(Robot *rb);
    bool removeRobot(Robot &rb);
    bool replaceRobot(Robot *oldRobot, Robot *newRobot);
    void displayList() const; // testing function
};

// RobotQueue class
class RobotQueue // have some bug, need to be fixed
{
private:
    RobotNode *front;
    RobotNode *rear;
    int numRobots;

public:
    RobotQueue();
    RobotQueue(const RobotQueue &rq);
    RobotQueue(RobotQueue &&rq);
    RobotQueue &operator=(const RobotQueue &rq);
    ~RobotQueue();

    // Queue operations
    void enqueue(Robot *);
    void dequeue(Robot *&);
    bool isEmpty() const;
    int getQueueLength() const { return numRobots; }
    void clear();
};

// Battlefield class
class Battlefield
{
private:
    int width, length;
    char emptyCell, boundary;
    Robot ***cellArr;

public:
    Battlefield();
    Battlefield(int w, int l, RobotList &rl); // initialize field using linked list

    Battlefield &operator=(const Battlefield &right);

    int getWidth() const { return width; }
    int getLength() const { return length; }

    void displayField() const;
    void displayField(ofstream &outfile) const;
    bool updatePosition(Robot *r, int x, int y); // need to swap pointer

    bool isEmpty(int x, int y) const { return cellArr[y][x] == nullptr; }
    bool isValid(int x, int y) const { return x >= 0 && x < width && y >= 0 && y < length; }

    void removeRobot(const Robot &r) { cellArr[r.getY()][r.getX()] = nullptr; }
    void removeRobot(int x, int y) { cellArr[y][x] = nullptr; }
    Robot *getRobotAt(int x, int y);

    ~Battlefield();
};

// MovingRobot class
class MovingRobot : virtual public Robot
{
private:
    enum DIRECTION
    {
        up,
        upright,
        right,
        downright,
        down,
        downleft,
        left,
        upleft
    };

public:
    MovingRobot() : Robot() {}
    MovingRobot(string n, string t, int x, int y, char s);
    virtual void move(Battlefield &bt, ofstream &outfile);
    virtual void takeTurn(Battlefield &bt, ofstream &outfile) = 0; // Pure virtual function
    virtual ~MovingRobot() {}
};

// ShootingRobot class
class ShootingRobot : virtual public Robot
{
public:
    ShootingRobot() : Robot() {}
    ShootingRobot(string t, string n, int x, int y, char s);

    virtual void fire(int offsetX, int offsetY, Battlefield &bt, ofstream &outfile);
    virtual void takeTurn(Battlefield &bt, ofstream &outfile) = 0; // Pure virtual function
    virtual ~ShootingRobot() {}
};

// SeeingRobot class
class SeeingRobot : virtual public Robot
{
private:
    int *RobotDetectedX;
    int *RobotDetectedY;
    int numOfRobotDetected;

public:
    SeeingRobot() : Robot() {}
    SeeingRobot(string t, string n, int x, int y, char s);
    int *getRobotDetectedX() const { return RobotDetectedX; }
    int *getRobotDetectedY() const { return RobotDetectedY; }
    bool getIsRobotDetected() const { return numOfRobotDetected != 0; }
    int getNumOfRobotDetected() const { return numOfRobotDetected; }
    void resetDetection();
    virtual void look(int offsetX, int offsetY, Battlefield &bt, ofstream &outfile);
    virtual void takeTurn(Battlefield &bt, ofstream &outfile) = 0; // Pure virtual function
    virtual ~SeeingRobot();
};

// SteppingRobot class
class SteppingRobot : virtual public Robot
{
private:
    int robotStepCount;

public:
    SteppingRobot(string t, string n, int x, int y, char s);
    virtual void step(int coordinateX, int coordinateY, Battlefield &bt, ofstream &outfile);
    int getRobotStepCount() const { return robotStepCount; }
    void resetRobotStepCount() { robotStepCount = 0; }
    virtual void takeTurn(Battlefield &bt, ofstream &outfile) = 0; // Pure virtual function
    virtual ~SteppingRobot() {}
};

// RoboCop class
class RoboCop : virtual public MovingRobot, virtual public ShootingRobot, virtual public SeeingRobot
{
public:
    RoboCop(string t, string n, int x, int y, char s);
    virtual void takeTurn(Battlefield &bt, ofstream &outfile);
};

// Terminator class
class Terminator : virtual public MovingRobot, virtual public SeeingRobot, virtual public SteppingRobot
{
public:
    Terminator(string t, string n, int x, int y, char s);
    virtual void takeTurn(Battlefield &bt, ofstream &outfile);
};

// BlueThunder class
class BlueThunder : virtual public ShootingRobot
{
private:
    int directionCount;

public:
    BlueThunder(string t, string n, int x, int y, char s);
    virtual void takeTurn(Battlefield &bt, ofstream &outfile);
};

// Madbot class
class Madbot : virtual public BlueThunder
{
public:
    Madbot(string t, string n, int x, int y, char s);
    virtual void takeTurn(Battlefield &bt, ofstream &outfile);
};

// TerminatorRoboCop class
class TerminatorRoboCop : virtual public RoboCop, virtual public Terminator
{
public:
    TerminatorRoboCop(string t, string n, int x, int y, char s);
    virtual void takeTurn(Battlefield &bt, ofstream &outfile);
};

// RoboTank class
class RoboTank : virtual public Madbot
{
public:
    RoboTank(string t, string n, int x, int y, char s);
    virtual void takeTurn(Battlefield &bt, ofstream &outfile);
};

// UltimateRobot class
class UltimateRobot : public TerminatorRoboCop, public RoboTank
{
public:
    UltimateRobot(string t, string n, int x, int y, char s);
    void takeTurn(Battlefield &bt, ofstream &outfile) override;
};

// Extra robot classes
// BomberRobot class        X
// Can shoot in consequent XXX area and destroy the robots
//                          X
class BomberMan : virtual public MovingRobot, virtual public ShootingRobot
{
private:
    bool movePermission;

public:
    BomberMan(string t, string n, int x, int y, char s);
    virtual void takeTurn(Battlefield &bt, ofstream &outfile);
    bool getMovePermission() { return movePermission; }
    void setMovePermission(bool m) { movePermission = m; }
    virtual ~BomberMan() {}
};

// HealingBomber class
// Increase its remaining lives by 1 every 10 rounds if its remaining lives is less than 3
class HealingBomber : virtual public BomberMan
{
private:
    int healingTurn;

public:
    HealingBomber(string t, string n, int x, int y, char s);
    virtual void takeTurn(Battlefield &bt, ofstream &outfile) override;
    void heal(ofstream &outfile);
    virtual ~HealingBomber() {}
};

// Terrorist class
// can see and step, if no step, move
// can bomb
// also can heal
class Terrorist : public HealingBomber, public SeeingRobot, public SteppingRobot
{
public:
    Terrorist(string t, string n, int x, int y, char s);
    void takeTurn(Battlefield &bt, ofstream &outfile) override;
    virtual ~Terrorist() {}
};

// War class
class War
{
private:
    Battlefield battlefield;
    int totalSteps;
    int currentStep;
    int totalRobots;
    int noOfRobotPlaying;
    int robotsRemaining;
    int robotsDied;

    RobotList robotPlaying;

    RobotQueue robotWaiting;
    int noOfRobotWaiting;

public:
    War(const string &input);
    void initializeRobot(string tt, string tn, int tx, int ty);
    ~War() {}

    bool isPlayingEmpty() const { return robotPlaying.getListLength() == 0; }
    bool isWaitingEmpty() const { return robotWaiting.getQueueLength() == 0; }

    void changePosition(Robot *&r);
    void terminateRobot(Robot &r, ofstream &outfile);
    void promoteRobot(Robot &r, ofstream &outfile);

    void startWar(const string &output);
};

#include "robot.cpp"
#endif