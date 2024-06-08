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
    int robotPositionX, robotPositionY;
    int remainingLives;
    Robot *robotTerminated[3];
    int numOfRobotTerminated;
    bool upgradePermission;

public:
    Robot();                                 // Default constructor
    Robot(string t, string n, int x, int y); // Parameterized constructor
    Robot(const Robot &r);                   // Copy constructor
    Robot &operator=(const Robot &right);    // Assignment operator overloading
    virtual ~Robot();                        // Destructor

    string getName() const { return robotName; } // Return robot name
    string getType() const { return robotType; } // Return robot type

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

    virtual void takeTurn(Battlefield &bt) = 0;                     // Pure virtual function
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
    RobotNode() : robot(nullptr), next(nullptr) {}                          // default constructor
    RobotNode(Robot &rb) : robot(&rb), next(nullptr) {}                     // parameterized constructor
    RobotNode(Robot &rb, RobotNode *nextPtr) : robot(&rb), next(nextPtr) {} // parameterized constructor

    RobotNode(const RobotNode &rn) : robot(rn.robot), next(rn.next) {} // copy constructor
    RobotNode(RobotNode &&rn);                                         // move constructor
    RobotNode &operator=(const RobotNode &rn);                         // copy assignment operator

    ~RobotNode() {}

    void setRobot(Robot &rb) { robot = &rb; }
    void setNext(RobotNode *nextPtr) { next = nextPtr; }
    Robot &getRobot() const { return *robot; }
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
    void appendRobot(Robot &rb);
    bool removeRobot(Robot &rb);
    bool replaceRobot(Robot &oldRobot, Robot &newRobot);
    void displayList() const; // testing function
};

// RobotQueue class
class RobotQueue
{
private:
    RobotNode *front;
    RobotNode *rear;
    int numRobots;

public:
    RobotQueue();

    ~RobotQueue();

    // Queue operations
    void enqueue(Robot *);
    void dequeue(Robot &);
    bool isEmpty() const;
    int getQueueLength() const {return numRobots;}
    void clear();
};

// Battlefield class
class Battlefield
{
private:
    int width, length;
    char emptyCell, robotCell, boundary;
    Robot ***cellArr;

public:
    Battlefield();
    Battlefield(int w, int l, RobotList *rn); // initialize field using linked list
    Battlefield &operator=(const Battlefield &right);

    int getWidth() const { return width; }
    int getLength() const { return length; }

    void displayField() const;
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
    MovingRobot(string n, string t, int x, int y);
    virtual void move(Battlefield &bt);
    virtual void takeTurn(Battlefield &bt) {} // Pure virtual function
    virtual ~MovingRobot() {}
};

// ShootingRobot class
class ShootingRobot : virtual public Robot
{
public:
    ShootingRobot() : Robot() {}
    ShootingRobot(string t, string n, int x, int y);

    virtual void fire(int offsetX, int offsetY, Battlefield &bt);
    virtual void takeTurn(Battlefield &bt) = 0; // Pure virtual function
    virtual ~ShootingRobot() {}
};

// SeeingRobot class
class SeeingRobot : virtual public Robot
{
private:
    int *RobotDetectedX;
    int *RobotDetectedY;
    bool isRobotDetected;
    int numOfRobotDetected;

public:
    SeeingRobot() : Robot() {}
    SeeingRobot(string t, string n, int x, int y);
    int *getRobotDetectedX() const { return RobotDetectedX; }
    int *getRobotDetectedY() const { return RobotDetectedY; }
    bool getIsRobotDetected() const { return isRobotDetected; }
    int getNumOfRobotDetected() const { return numOfRobotDetected; }
    void resetDetection();
    virtual void look(int offsetX, int offsetY, Battlefield &bt);
    virtual void takeTurn(Battlefield &bt) = 0; // Pure virtual function
    virtual ~SeeingRobot();
};

// SteppingRobot class
class SteppingRobot : virtual public Robot
{
private:
    int robotStepCount;

public:
    SteppingRobot(string t, string n, int x, int y);
    virtual void step(int coordinateX, int coordinateY, Battlefield &bt);
    int getRobotStepCount() const { return robotStepCount; }
    void resetRobotStepCount() { robotStepCount = 0; }
    virtual void takeTurn(Battlefield &bt) = 0; // Pure virtual function
    virtual ~SteppingRobot() {}
};

// RoboCop class
class RoboCop : virtual public MovingRobot, virtual public ShootingRobot, virtual public SeeingRobot
{
public:
    RoboCop(string t, string n, int x, int y);
    virtual void takeTurn(Battlefield &bt);
};

// Terminator class
class Terminator : virtual public MovingRobot, virtual public SeeingRobot, virtual public SteppingRobot
{
public:
    Terminator(string t, string n, int x, int y);
    virtual void takeTurn(Battlefield &bt);
};

// BlueThunder class
class BlueThunder : virtual public ShootingRobot
{
private:
    int directionCount;

public:
    BlueThunder(string t, string n, int x, int y);
    virtual void takeTurn(Battlefield &bt);
};

// Madbot class
class Madbot : virtual public BlueThunder
{
public:
    Madbot(string t, string n, int x, int y);
    virtual void takeTurn(Battlefield &bt);
};

// TerminatorRoboCop class
class TerminatorRoboCop : virtual public RoboCop, virtual public Terminator
{
public:
    TerminatorRoboCop(string t, string n, int x, int y);
    virtual void takeTurn(Battlefield &bt);
};

// RoboTank class
class RoboTank : virtual public Madbot
{
public:
    RoboTank(string t, string n, int x, int y);
    virtual void takeTurn(Battlefield &bt);
};

// UltimateRobot class
class UltimateRobot : public TerminatorRoboCop, public RoboTank
{
public:
    UltimateRobot(string t, string n, int x, int y);
    virtual void takeTurn(Battlefield &bt);
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

    RobotList *robotPlaying;

    RobotQueue robotWaiting;
    int noOfRobotWaiting;

public:
    War(const string &filename);
    void initializeRobot(string tt, string tn, int tx, int ty);
    ~War();

    bool isPlayingEmpty() const;

    void enqueueWaiting(Robot &r);
    void dequeueWaiting(Robot &r);
    bool isWaitingEmpty() const;

    void terminateRobot(Robot &r);
    void promoteRobot(Robot &r);

    void startWar();
};

#endif