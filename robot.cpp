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
    return cellArr[x][y] == ' ';
}

bool Battlefield::isValid(int x, int y) const
{
    return x > 0 && x < width && y > 0 && y < length && isEmpty(x, y);
}

void Battlefield::removeRobot(const Robot &r)
{
    cellArr[r.getX()][r.getY()] = ' ';
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
Robot::Robot(string n, string t, Battlefield *bt) // Parameterized constructor
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

bool Robot::validPosition(int x, int y)
{
    battlefield->isValid(x, y);
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
MovingRobot::MovingRobot(string n, string t, Battlefield *bt) : Robot(n, t, bt)
{
}

void MovingRobot::move()
{
    int newX, newY;
    DIRECTION direction;
    bool invalidMove = true;
    srand(time(0));
    while (invalidMove)
    {
        direction = static_cast<DIRECTION>(rand() % 8);
        switch (direction)
        {
        case up:
            newY = getY() + 1;
            if(validPosition(getX(), newY))
            {
                setY(newY);
                invalidMove = false;
            }    
            else
                continue;
            break;
        case upright:
            newX = getX() + 1;
            newY = getY() + 1;
            if(validPosition(newX, newY))
            {
                setX(newX);
                setY(newY);
            }
            else
                continue;
            break;
        case right:
            newX = getX() + 1;
            if(validPosition(newX, getY()))
            {
                setX(newX);
                invalidMove = false;
            }  
            else
                continue;
            break;
        case downright:
            newX = getX() + 1;
            newY = getY() - 1;
            if(validPosition(newX, newY))
            {
                setX(newX);
                setY(newY);
                invalidMove = false;
            }
            else
                continue;
            break;
        case down:
            newY = getY() - 1;
            if(validPosition(getX(), newY))
            {
                setY(newY);
                invalidMove = false;
            }    
            else
                continue;
            break;
        case downleft:
            newX = getX() - 1;
            newY = getY() - 1;
            if(validPosition(newX, newY))
            {
                setX(newX);
                setY(newY);
                invalidMove = false;
            }
            else
                continue;
            break;
        case left:
            newX = getX() - 1;
            if(validPosition(newX, getY()))
            {
                setX(newX);
                invalidMove = false;
            }    
            else
                continue;
            break;
        case upleft:
            newX = getX() - 1;
            newY = getY() + 1;
            if(validPosition(newX, newY))
            {
                setX(newX);
                setY(newY);
                invalidMove = false;
            }
            else
                continue;
        }
    }
}
