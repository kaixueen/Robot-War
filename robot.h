#ifndef ROBOT_H
#define ROBOT_H
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class War
{
private:
    Battlefield *battlefield;
    int totalSteps;
    int currentStep;
    int totalRobots;
    int robotsRemaining;
    int robotsDied;
    struct RobotPlaying
    {
        Robot *player;
        RobotPlaying *next;
    };
    RobotPlaying *frontPlaying;
    RobotPlaying *rearPlaying;
    int noOfRobotPlaying;
    void enqueuePlaying(Robot *r);
    void dequeuePlaying();
    bool isPlayingEmpty() const;

    struct RobotWaiting
    {
        Robot *player;
        RobotWaiting *next;
    };
    RobotWaiting *frontWaiting;
    RobotWaiting *rearWaiting;
    int noOfRobotWaiting;
    void enqueueWaiting(Robot *r);
    void dequeueWaiting();
    bool isWaitingEmpty() const;

public:
    War(const string &filename);
    ~War();

    void robotKilled(Robot &r);
    void startWar();
    int
};

class Battlefield
{
private:
    int width, length;
    char emptyCell, robotCell, boundary;
    Robot ***cellArr;

public:
    Battlefield(int w, int l, Robot *r[], int tr); // initialize field

    void displayField() const;
    bool updatePosition(Robot *r, int x, int y); // need to swap pointer
    bool isEmpty(int x, int y) const;
    bool isValid(int x, int y) const;
    void removeRobot(const Robot &r);
    Robot &getRobotAt(int x, int y);

    ~Battlefield();
};

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
    bool validPosition(int x, int y, Battlefield *bt) const;
    bool enemyExist(int x, int y, Battlefield *bt) const;

    bool stillGotLive() const;
    void setRemainingLives(int l);
    int getRemainingLives() const;
};

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
class ShootingRobot : public Robot
{
};

class SeeingRobot : public Robot
{
};

class SteppingRobot : public Robot
{
};

class RoboCop : public MovingRobot, public ShootingRobot, public SeeingRobot
{
};

class Terminator : public MovingRobot, public SeeingRobot, public SteppingRobot
{
};

class BigThunder : public ShootingRobot
{
};

class MadBot : public BigThunder
{
};

class TerminatorRoboCop : public RoboCop, public Terminator
{
};

class RoboTank : public MadBot
{
};

class UltimateRobot : public TerminatorRoboCop, public RoboTank
{
};

#endif
