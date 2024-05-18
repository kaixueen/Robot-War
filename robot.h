#ifndef ROBOT_H
#define ROBOT_H
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class War
{};
class Battlefield
{
private:
    int width, length;
    char emptyCell, robotCell, boundary;
    char **cellArr;

public:
    Battlefield(int w, int l);

    void displayField() const;
    void updateField(const Robot &r);
    bool isEmpty() const;
    void removeRobot(const Robot& r);
    
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
    virtual ~Robot();                           // Destructor

    virtual void move() = 0;
    virtual void fire() = 0;
    virtual void look() = 0;
    virtual void step() = 0;

    int getX() const;
    int getY() const;
    void setX();
    void setY();
    bool isAlive() const;
    int getRemainingLives() const;
    int getRemainingHP() const;
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