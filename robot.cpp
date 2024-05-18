#include <iostream>
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

Battlefield::~Battlefield()
{
    for (int i = 0; i < length; i++)
    {
        delete[] cellArr[i];
    }
    delete[] cellArr;
}

// Robot class
Robot::Robot()                            // Default constructor
{
    robotName = robotType = "";
    robotPositionX = robotPositionY = -1;
    remainingLives = 3;
    remainingHP = 100;
}

Robot::Robot(string n, string t)          // Parameterized constructor
{
    robotName = n;
    robotType = t;
    robotPositionX = robotPositionY = -1;
    remainingLives = 3;
    remainingHP = 100;
}

Robot::Robot(const Robot &r)               // Copy constructor
{
    robotName = r.robotName;
    robotType = r.robotType;
    robotPositionX = r.robotPositionX;
    robotPositionY = r.robotPositionY;
    remainingLives = r.remainingLives;
    remainingHP = r.remainingHP;
}

Robot & Robot::operator=(const Robot &right) // Assignment operator overloading
{
    if (this != &right)
    {
        robotName = right.robotName;
        robotType = right.robotType;
        robotPositionX = right.robotPositionX;
        robotPositionY = right.robotPositionY;
        remainingLives = right.remainingLives;
        remainingHP = right.remainingHP;
    }
    return *this;
}

int Robot::getX() const
{
    return robotPositionX;
}

int Robot::getY() const
{
    return robotPositionY;
}

void Robot::setX(int &x)
{
    robotPositionX = x;
}

void Robot::setY(int &y)
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

