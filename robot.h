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

    string getName() const; // Return robot name
    string getType() const; // Return robot type

    int getX() const; // Return robot x coordinate
    int getY() const; // Return robot y coordinate
    void setX(int x); // Set robot x coordinate
    void setY(int y); // Set robot y coordinate

    bool stillGotLive() const;     // Remaining lives > 0?
    void setRemainingLives(int l); // Set remaining lives
    int getRemainingLives() const; // Return remaining lives

    void setRobotTerminated(Robot &r);      // store robot terminated
    Robot &getRobotTerminated(int n) const; // return robot terminated
    int getNumOfRobotTerminated() const;    // return number of robot terminated
    void resetRobotTerminated();            // reset robot terminated to nullptr

    virtual void takeTurn(Battlefield &bt) = 0; // Pure virtual function
    bool getUpgradePermission() const;          // Valid for upgrade?
    void setUpgradePermission(bool p);          // Set upgradePermission
};

struct RobotNode // struct for making linked list & queue
{
    Robot *rb;
    RobotNode *nextRobot;
    RobotNode() : rb(nullptr), nextRobot(nullptr) {}
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
    Battlefield(int w, int l, RobotNode *rn); // initialize field using linked list

    int getWidth() const;
    int getLength() const;

    void displayField() const;
    bool updatePosition(Robot *r, int x, int y); // need to swap pointer

    bool isEmpty(int x, int y) const;
    bool isValid(int x, int y) const;

    void removeRobot(const Robot &r);
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
    int *getRobotDetectedX() const;
    int *getRobotDetectedY() const;
    bool getIsRobotDetected() const;
    int getNumOfRobotDetected() const;
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
    int getRobotStepCount() const;
    void resetRobotStepCount();
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
private:
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

    RobotNode *headRobot; // linked list of robot playing

    RobotNode *frontWaiting; // queue of robot waiting
    RobotNode *rearWaiting;
    int noOfRobotWaiting;

public:
    War(const string &filename);
    void initializeRobot(string tt, string tn, int tx, int ty);
    ~War();

    Robot *getRobotPlaying(int i);

    void appendRobot(Robot &r);
    void deleteRobot(Robot &r);
    bool isPlayingEmpty() const;

    void enqueueWaiting(Robot &r);
    void dequeueWaiting(Robot &r);
    bool isWaitingEmpty() const;

    void terminateRobot(Robot &r);
    void promoteRobot(Robot &r);
    void switchRobot(Robot &r1, Robot &r2);
    void startWar();
};

#endif
