#include <iostream>
#include <cstdlib>
#include <time.h>
#include "robot.h"
using namespace std;

// War class
War::War(const string &filename)
{
    ifstream infile(filename);
    if (infile.fail())
    {
        cout << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    string line;

    // Read battlefield dimensions
    getline(infile, line);
    stringstream ss(line);
    string temp;
    int w, l;

    while (!ss.eof())
    {
        ss >> temp;
        if (stringstream(temp) >> w)
        {
            ss >> l;
            break;
        }
    }

    // Read number of steps
    getline(infile, line);
    ss.clear();
    int steps;
    while (!ss.eof())
    {
        ss >> temp;
        if (stringstream(temp) >> steps)
            break;
    }

    // Read number of steps
    getline(infile, line);
    ss.clear();
    int numRobots;
    while (!ss.eof())
    {
        ss >> temp;
        if (stringstream(temp) >> numRobots)
            break;
    }

    // Read information of robots
    ss.clear();
    string tempType;
    string tempName;
    string tempX;
    string tempY;
    Robot *robots[numRobots];
    int count = 0;
    while (getline(infile, line))
    {
        ss >> tempType >> tempName;
        ss >> tempX >> tempY;
        Robot *newRobot = new Robot(tempType, tempName, tempX, tempY); // will change to each robot type later
        robots[count] = newRobot;
        ss.clear();
        newRobot = nullptr;
        count++;
    }

    // Initialize playing queue
    frontPlaying = rearPlaying = nullptr;
    noOfRobotPlaying = numRobots;
    for (int i = 0; i < numRobots; i++)
    {
        enqueuePlaying(robots[i]);
    }

    // Initialize waiting queue
    frontWaiting = rearWaiting = nullptr;
    noOfRobotWaiting = 0;

    // Initialize other data member
    battlefield = new Battlefield(w, l, robots, numRobots);
    totalSteps = steps;
    currentStep = 0;
    totalRobots = robotsRemaining = numRobots;
    robotsDied = 0;

    for (int i = 0; i < numRobots; i++)
    {
        robots[i] = nullptr;
        delete robots[i];
    }
    delete[] robots;
}

void War::enqueuePlaying(Robot *r)
{
    RobotPlaying *newRobot = nullptr;
    newRobot = new RobotPlaying;
}

// Battlefield class
Battlefield::Battlefield(int w, int l, Robot *r[], int tr)
{
    width = w;
    length = l;
    emptyCell = ' ';
    robotCell = 'R';
    boundary = '*';
    cellArr = new Robot **[length];

    int tempX, tempY;

    for (int i = 0; i < length; i++)
    {
        cellArr[i] = new Robot *[width];
        for (int j = 0; j < width; j++)
        {
            cellArr[i][j] = nullptr;
        }
    }
    for (int i = 0; i < tr; i++)
    {
        tempY = r[i]->getY();
        tempX = r[i]->getX();
        if (isValid(tempY, tempX) && isEmpty(tempY, tempX))
            cellArr[tempY][tempX] = r[i];
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
            {
                if (cellArr[i][j] == nullptr)
                    cout << emptyCell;
                else
                    cout << robotCell;
            }
        }
        cout << endl;
    }
}

bool Battlefield::updatePosition(Robot *r, int x, int y)
{
    if (isValid(x, y) && isEmpty(x, y))
    {
        cellArr[r->getY()][r->getX()] = nullptr;
        cellArr[y][x] = r;
        r->setX(x);
        r->setY(y);
        return true;
    }
    return false;
}

bool Battlefield::isEmpty(int x, int y) const
{
    return cellArr[y][x] == nullptr;
}

bool Battlefield::isValid(int x, int y) const
{
    return x > 0 && x < width && y > 0 && y < length;
}

void Battlefield::removeRobot(const Robot &r)
{
    cellArr[r.getY()][r.getX()] = nullptr;
}

Robot &Battlefield::getRobotAt(int x, int y)
{
    if (isValid(x, y) && isEmpty(x, y))
        return *cellArr[y][x];
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
Robot::Robot()
{
    robotName = "";
    robotType = "";
    robotPositionX = robotPositionY = -1;
    remainingLives = 3;
}

Robot::Robot(string t, string n, int x, int y) // Parameterized constructor
{
    robotName = n;
    robotType = t;
    robotPositionX = x;
    robotPositionY = y;
    remainingLives = 3;
}

Robot::Robot(const Robot &r) // Copy constructor
{
    robotName = r.robotName;
    robotType = r.robotType;
    robotPositionX = r.robotPositionX;
    robotPositionY = r.robotPositionY;
    remainingLives = r.remainingLives;
}

Robot &Robot::operator=(const Robot &right) // Assignment operator overloading
{
    if (this != &right)
    {
        robotName = right.robotName;
        robotType = right.robotType;
        robotPositionX = right.robotPositionX;
        robotPositionY = right.robotPositionY;
        remainingLives = right.remainingLives;
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

bool Robot::validPosition(int x, int y, Battlefield *bt) const
{
    return bt->isValid(x, y);
}

bool Robot::enemyExist(int x, int y, Battlefield *bt) const
{
    return !(bt->isEmpty(x, y));
}

bool Robot::stillGotLive() const
{
    return remainingLives > 0;
}

int Robot::getRemainingLives() const
{
    return remainingLives;
}

void Robot::setRemainingLives(int l)
{
    remainingLives = l;
}

// MovingRobot
MovingRobot::MovingRobot(string n, string t, int x, int y) : Robot(n, t, x, y)
{
}

void MovingRobot::move(Battlefield *bt)
{
    int newX, newY;
    DIRECTION direction;
    bool invalidMove = true;
    srand(time(0));
    while (invalidMove)
    {
        newX = getX();
        newY = getY();
        direction = static_cast<DIRECTION>(rand() % 8);
        switch (direction)
        {
        case up:
            newY += 1;
            break;
        case upright:
            newX += 1;
            newY += 1;
            break;
        case right:
            newX += 1;
            break;
        case downright:
            newX += 1;
            newY -= 1;
            break;
        case down:
            newY -= 1;
            break;
        case downleft:
            newX -= 1;
            newY -= 1;
            break;
        case left:
            newX -= 1;
            break;
        case upleft:
            newX -= 1;
            newY += 1;
        }
        if (validPosition(newX, newY, bt) && !(enemyExist(newX, newY, bt)))
        {
            setX(newX);
            setY(newY);
            invalidMove = false;
        }
    }
    cout << getType() << " " << getName() << " move to (" << newX << ", " << newY << ").";
}
