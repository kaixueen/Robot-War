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
class MovingRobot : public Robot
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
    virtual void move(Battlefield *bt);
    virtual ~MovingRobot() {}
};

// ShootingRobot class
class ShootingRobot : public Robot
{
};

// SeeingRobot class
class SeeingRobot : public Robot
{
};

// SteppingRobot class
class SteppingRobot : public Robot
{
};

// RoboCop class
class RoboCop : public MovingRobot, public ShootingRobot, public SeeingRobot
{
};

// Terminator class
class Terminator : public MovingRobot, public SeeingRobot, public SteppingRobot
{
};

// BigThunder class
class BigThunder : public ShootingRobot
{
};

// MadBot class
class MadBot : public BigThunder
{
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
    Battlefield *battlefield;
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

    void appendRobot(Robot *r);
    void deleteRobot(Robot *r);
    bool isPlayingEmpty() const;

    void enqueueWaiting(Robot *r);
    void dequeueWaiting(Robot &r);
    bool isWaitingEmpty() const;

    void robotKilled(Robot *r);
    void startWar();
};

#endif