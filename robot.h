#ifndef ROBOT_H
#define ROBOT_H
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

// Robot class
class Robot
{
private:
    string robotName;
    string robotType;
    int robotPositionX, robotPositionY;
    int remainingLives;
    bool upgradePermission;

public:
    Robot();
    Robot(string t, string n, int x, int y); // Parameterized constructor
    Robot(const Robot &r);                   // Copy constructor
    Robot &operator=(const Robot &right);    // Assignment operator overloading
    virtual ~Robot() {}                      // Destructor

    virtual void move() = 0;
    virtual void fire() = 0;
    virtual void look() = 0;
    virtual void step() = 0;

    string getName() const;
    string getType() const;

    int getX() const;
    int getY() const;
    void setX(int x);
    void setY(int y);

    bool stillGotLive() const;
    void setRemainingLives(int l);
    int getRemainingLives() const;

    virtual void takeAction() = 0; // new pure virtual function
    bool getUpgradePermission() const;
    void setUpgradePermission(bool p);
};

struct RobotNode // struct for making linked list & queue
{
    Robot *rb;
    RobotNode *nextRobot;
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
    Battlefield(int w, int l, RobotNode *rn); // initialize field

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
    const enum DIRECTION { up,
                           upright,
                           right,
                           downright,
                           down,
                           downleft,
                           left,
                           upleft };

public:
    MovingRobot() : Robot() {}
    MovingRobot(string n, string t, int x, int y);
    virtual void move(Battlefield &bt);
    virtual ~MovingRobot() {}
};

// ShootingRobot class
class ShootingRobot : virtual public Robot
{
private:
    bool notValid; // need to rename
    Robot *robotShot;
    int robotShotCount;
    int fireCount;

public:
    ShootingRobot() : Robot() {}
    ShootingRobot(string t, string n, int x, int y);
    bool fireNotValid() const; // need to rename

    void incFireCount();
    int getFireCount() const;

    void setRobotShot(Robot &r);      // store robot shot
    Robot &getRobotShot(int n) const; // return robot shot
    int getRobotShotCount() const;

    virtual void fire(int offsetX, int offsetY, Battlefield &bt);
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
    virtual ~SeeingRobot() {}
};

// SteppingRobot class
class SteppingRobot : virtual public Robot
{
private:
    Robot *robotStep;
    void setRobotStep(Robot &r);
    Robot *getRobotStep() const;
    int robotStepCount;

public:
    SteppingRobot(string t, string n, int x, int y);
    int getStepCount() const;
    void incStepCount();
    virtual void step(int coordinateX, int coordinateY, Battlefield &bt);
    virtual ~SteppingRobot() {}
};

// RoboCop class
class RoboCop : public MovingRobot, public ShootingRobot, public SeeingRobot
{
private:
    int fireCount;

public:
    RoboCop(string t, string n, int x, int y);
    void takeTurn(Battlefield &bt); // need to modify
};

// Terminator class
class Terminator : public MovingRobot, public SeeingRobot, public SteppingRobot
{
private:
public:
    Terminator(string t, string n, int x, int y);
    void takeTurn(Battlefield &bt);
};

// BlueThunder class
class BlueThunder : public ShootingRobot
{
private:
    int directionCount;

public:
    BlueThunder(string t, string n, int x, int y);
    void takeTurn(Battlefield &bt);
};

// MadBot class
class MadBot : public BlueThunder
{
public:
    MadBot(string t, string n, int x, int y);
    void takeTurn(Battlefield &bt);
};

// TerminatorRoboCop class
class TerminatorRoboCop : public RoboCop, public Terminator
{
};

// RoboTank class
class RoboTank : public MadBot
{
};

// UltimateRobot class
class UltimateRobot : public TerminatorRoboCop, public RoboTank
{
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
    ~War();

    Robot *getRobotPlaying(int i);

    void appendRobot(Robot &r);
    void deleteRobot(Robot &r);
    bool isPlayingEmpty() const;

    void enqueueWaiting(Robot &r);
    void dequeueWaiting(Robot &r);
    bool isWaitingEmpty() const;

    void robotKilled(Robot &r);
    void promoteRobot(Robot &r);
    void switchRobot(Robot &r1, Robot &r2);
    void startWar();
};

#endif
