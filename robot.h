#ifndef ROBOT_H
#define ROBOT_H
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class War
{
};
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
    bool isEmpty(int x, int y) const;
    bool isValid(int x, int y) const;
    void removeRobot(const Robot &r);

    ~Battlefield();
};

class Robot
{
private:
    string robotName;
    string robotType;
    int robotPositionX, robotPositionY;
    int remainingLives;
    int remainingHP;
    Battlefield* battlefield;

public:
    Robot(string n, string t, Battlefield* bt);            // Parameterized constructor
    Robot(const Robot &r);                // Copy constructor
    Robot &operator=(const Robot &right); // Assignment operator overloading
    virtual ~Robot() {}                   // Destructor

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
    bool validPosition(int x, int y);
    bool enemyExist(int x, int y);

    bool isAlive() const;
    bool stillGotLive() const;

    int getRemainingLives() const;
    int getRemainingHP() const;
};

class MovingRobot : public Robot
{
private:
    const enum DIRECTION {up, upright, right, downright, down, downleft, left, upleft};

public:
    MovingRobot(string n, string t, Battlefield* bt);
    virtual void move();
    virtual ~MovingRobot() {}
};

class ShootingRobot : public Robot
{
};

class SeeingRobot : public Robot
{
public:
    SeeingRobot(string n, string t, Battlefield* bt);
    virtual void look(int offsetX, int offsetY) const override;
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
