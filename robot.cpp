#include <iostream>
#include <cstdlib>
#include <time.h>
#include "robot.h"
using namespace std;

// Battlefield class
Battlefield::Battlefield(int w, int l)
{
    width = w;
    length = l;
    emptyCell = ' ';
    robotCell = 'R';
    boundary = '*';
    cellArr = new char *[length];

    for (int i = 0; i < length; i++)
    {
        cellArr[i] = new char[w];
        for (int j = 0; j < width; j++)
        {
            cellArr[i][j] = emptyCell;
        }
    }
}

void Battlefield::displayField() const
{
    for (int i = 0; i < length + 2; i++)
    {
        for (int j = 0; j < width + 2; j++)
        {
            if (i == 0 || i == length + 1 || j == 0 || j == width + 1)
                cout << boundary;
            else
                cout << cellArr[i - 1][j - 1];
        }
        cout << endl;
    }
}

void Battlefield::updateField(const Robot &r)
{
    cellArr[r.getX()][r.getY()] = robotCell;
}

bool Battlefield::isEmpty(int x, int y) const
{
    
}

Battlefield::~Battlefield()
{
    for (int i = 0; i < length; i++)
    {
        delete[] cellArr[i];
    }
    delete[] cellArr;
}

// Robot class
Robot::Robot(string n, string t, Battlefield* bt) // Parameterized constructor
{
    robotName = n;
    robotType = t;
    battlefield = bt;
    robotPositionX = robotPositionY = -1;
    remainingLives = 3;
    remainingHP = 100;
}

Robot::Robot(const Robot &r) // Copy constructor
{
    robotName = r.robotName;
    robotType = r.robotType;
    battlefield = r.battlefield;
    robotPositionX = r.robotPositionX;
    robotPositionY = r.robotPositionY;
    remainingLives = r.remainingLives;
    remainingHP = r.remainingHP;
}

Robot &Robot::operator=(const Robot &right) // Assignment operator overloading
{
    if (this != &right)
    {
        robotName = right.robotName;
        robotType = right.robotType;
        battlefield = right.battlefield;
        robotPositionX = right.robotPositionX;
        robotPositionY = right.robotPositionY;
        remainingLives = right.remainingLives;
        remainingHP = right.remainingHP;
    }
    return *this;
}

string Robot::getName() const
{
    return robotName;
}

string Robot::getType() const
{
    return robotType;
}

int Robot::getX() const
{
    return robotPositionX;
}

int Robot::getY() const
{
    return robotPositionY;
}

void Robot::setX(int x)
{
    robotPositionX = x;
}

void Robot::setY(int y)
{
    robotPositionY = y;
}

bool Robot::isAlive() const
{
    return remainingHP > 0;
}

bool Robot::stillGotLive() const
{
    return remainingLives > 0;
}

int Robot::getRemainingLives() const
{
    return remainingLives;
}

int Robot::getRemainingHP() const
{
    return remainingHP;
}

// MovingRobot
MovingRobot::MovingRobot(string n, string t) : Robot(n, t)
{
}

void MovingRobot::move()
{
    srand(time(0));
    DIRECTION direction = static_cast<DIRECTION>(rand() % 8);
    switch (direction)
    {
        case up:
            setY(getY()+1);
            break;
        case upright:
            setX(getX()+1);
            setY(getY()+1);
            break;
        case right:
            setX(getX()+1);
            break;
        case downright:
            setX(getX()+1);
            setY(getY()-1);
            break;
        case down:
            setY(getY()-1);
            break;
        case downleft:
            setX(getX()-1);
            setY(getY()-1);
            break;
        case left:
            setX(getX()-1);
            break;
        case upleft:
            setX(getX()-1);
            setY(getY()+1);
    }
}