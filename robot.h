#ifndef ROBOT_H
#define ROBOT_H
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Battlefield
{
private:
    int Width, Length;
    char RepresentSymbol;

public:
    Battlefield();
    ~Battlefield();
};

class Robot
{
private:
    string RobotName;
    string RobotType;
    int RobotPositionX, RobotPositionY;
    int RemainingLives;
    int RemainingHP;

public:
    Robot();                            // Default constructor
    Robot(string n, string t);          // Parameterized constructor
    Robot(const Robot &);               // Copy constructor
    Robot *operator=(const Robot *obj); // Assignment operator overloading
    Robot(Robot &&);                    // Move constructor
    ~Robot();                           // Destructor

    virtual void move() = 0;
    virtual void fire() = 0;
    virtual void look() = 0;
    virtual void step() = 0;
};

class MovingRobot : public Robot
{
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