#include <iostream>
#include <cstdlib>
#include <time.h>
#include "robot.h"
using namespace std;

// Robot class
Robot::Robot()
{
    robotName = "";
    robotType = "";
    robotPositionX = robotPositionY = -1;
    remainingLives = 3;
    upgradePermission = false;
    numOfRobotTerminated = 0;
    for (int i = 0; i < 3; i++)
        robotTerminated[i] = nullptr;
}

Robot::Robot(string t, string n, int x, int y) // Parameterized constructor
{
    robotName = n;
    robotType = t;
    robotPositionX = x;
    robotPositionY = y;
    remainingLives = 3;
    upgradePermission = false;
    numOfRobotTerminated = 0;
    for (int i = 0; i < 3; i++)
        robotTerminated[i] = nullptr;
}

Robot::Robot(const Robot &r) // Copy constructor
{
    robotName = r.robotName;
    robotType = r.robotType;
    robotPositionX = r.robotPositionX;
    robotPositionY = r.robotPositionY;
    remainingLives = r.remainingLives;
    upgradePermission = r.upgradePermission;
    numOfRobotTerminated = r.numOfRobotTerminated;
    for (int i = 0; i < 3; i++)
        robotTerminated[i] = r.robotTerminated[i];
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
        upgradePermission = right.upgradePermission;
        numOfRobotTerminated = right.numOfRobotTerminated;
        for (int i = 0; i < 3; i++)
            robotTerminated[i] = right.robotTerminated[i];
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

void Robot::setRobotTerminated(Robot &r)
{
    robotTerminated[numOfRobotTerminated] = &r;
    numOfRobotTerminated++;
}

Robot &Robot::getRobotTerminated(int n) const
{
    return *robotTerminated[n];
}

int Robot::getNumOfRobotTerminated() const
{
    return numOfRobotTerminated;
}

void Robot::resetRobotTerminated()
{
    numOfRobotTerminated = 0;
    for (int i = 0; i < 3; i++)
        robotTerminated[i] = nullptr;
}

bool Robot::getUpgradePermission() const
{
    return upgradePermission;
}

void Robot::setUpgradePermission(bool p)
{
    upgradePermission = p;
}

Robot::~Robot()
{
    delete[] robotTerminated;
}

// Battlefield class
Battlefield::Battlefield()
{
    width = 0;
    length = 0;
    emptyCell = robotCell = boundary = ' ';
    cellArr = nullptr;
}

Battlefield::Battlefield(int w, int l, RobotNode *rn)
{
    width = w;
    length = l;
    emptyCell = ' ';
    robotCell = 'R';
    boundary = '*';
    cellArr = new Robot **[length];

    for (int i = 0; i < length; i++)
    {
        cellArr[i] = new Robot *[width];
        for (int j = 0; j < width; j++)
        {
            cellArr[i][j] = nullptr;
        }
    }

    if (rn == nullptr)
        return;

    int tempX, tempY;
    Robot *tempR;
    while (rn != nullptr)
    {
        tempR = rn->rb;
        tempY = tempR->getY();
        tempX = tempR->getX();
        if (isValid(tempY, tempX) && isEmpty(tempY, tempX))
            cellArr[tempY][tempX] = tempR;
        rn = rn->nextRobot;
    }
}

int Battlefield::getWidth() const
{
    return width;
}

int Battlefield::getLength() const
{
    return length;
}

void Battlefield::displayField() const
{
    for (int i = 0; i < width + 2; i++)
        cout << boundary;
    cout << endl;

    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0)
                cout << boundary;

            if (cellArr[i][j] == nullptr)
                cout << emptyCell;
            else
                cout << robotCell;

            if (j == width - 1)
                cout << boundary << endl;
        }
    }

    for (int i = 0; i < width + 2; i++)
        cout << boundary;
    cout << endl;
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
    return x >= 0 && x < width && y >= 0 && y < length;
}

void Battlefield::removeRobot(const Robot &r)
{
    cellArr[r.getY()][r.getX()] = nullptr;
}

Robot *Battlefield::getRobotAt(int x, int y)
{
    if (isValid(x, y) && !isEmpty(x, y))
        return cellArr[y][x];
    return nullptr;
}

Battlefield::~Battlefield()
{
    for (int i = 0; i < length; i++)
    {
        delete[] cellArr[i];
    }
    delete[] cellArr;
}

// MovingRobot class
MovingRobot::MovingRobot(string n, string t, int x, int y) : Robot(n, t, x, y)
{
}

void MovingRobot::move(Battlefield &bt)
{
    int newX, newY;
    DIRECTION direction;
    bool invalidMove = true;
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
        if (bt.isValid(newX, newY) && bt.isEmpty(newX, newY))
        {
            setX(newX);
            setY(newY);
            invalidMove = false;
        }
    }
    cout << getType() << " " << getName() << " move to (" << newX << ", " << newY << ").";
}

// ShootingRobot class
ShootingRobot::ShootingRobot(string t, string n, int x, int y) : Robot(t, n, x, y)
{
}

void ShootingRobot::fire(int offsetX, int offsetY, Battlefield &bt)
{
    int targetX = getX() + offsetX;
    int targetY = getY() + offsetY;

    if (!(offsetX == 0 && offsetY == 0) && bt.isValid(targetX, targetY))
    {
        if (!bt.isEmpty(targetX, targetY))
        {
            Robot *enemyRobot = bt.getRobotAt(targetX, targetY);
            cout << getType() << " " << getName() << " fires at (" << targetX << ", " << targetY << ") and destroys " << enemyRobot->getType() << " " << enemyRobot->getName() << "!" << endl;
            setRobotTerminated(*enemyRobot);
        }
        else
        {
            cout << getType() << " " << getName() << " fires at (" << targetX << ", " << targetY << ") but the position is empty." << endl;
        }
    }
}

// SeeingRobot class
SeeingRobot::SeeingRobot(string t, string n, int x, int y) : Robot(t, n, x, y)
{
    isRobotDetected = false;
    RobotDetectedX = new int[9];
    RobotDetectedY = new int[9];
    numOfRobotDetected = 0;

    for (int i = 0; i < 9; i++)
    {
        RobotDetectedX[i] = -1;
        RobotDetectedY[i] = -1;
    }
}

int *SeeingRobot::getRobotDetectedX() const
{
    return RobotDetectedX;
}

int *SeeingRobot::getRobotDetectedY() const
{
    return RobotDetectedY;
}

bool SeeingRobot::getIsRobotDetected() const
{
    return isRobotDetected;
}

int SeeingRobot::getNumOfRobotDetected() const
{
    return numOfRobotDetected;
}

void SeeingRobot::resetDetection()
{
    for (int i = 0; i < 9; i++)
    {
        RobotDetectedX[i] = -1;
        RobotDetectedY[i] = -1;
    }
    numOfRobotDetected = 0;
}

void SeeingRobot::look(int offsetX, int offsetY, Battlefield &bt) // need to reset the robot detected every turn, does it look at the offset position, if not only immediate neighbourhood
{
    int centerX = getX() + offsetX;
    int centerY = getY() + offsetY;

    cout << getType() << " " << getName() << " is looking around:" << endl;
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            int checkX = centerX + dx;
            int checkY = centerY + dy;

            if (bt.isValid(checkX, checkY))
            {
                cout << "Position (" << checkX << ", " << checkY << ") is within the battlefield and ";
                if (bt.isEmpty(checkX, checkY))
                {
                    cout << "is empty." << endl;
                }
                else
                {
                    cout << "contains an enemy robot." << endl;
                    isRobotDetected = true;
                    RobotDetectedX[numOfRobotDetected] = checkX;
                    RobotDetectedY[numOfRobotDetected] = checkY;
                    numOfRobotDetected++;
                }
            }
            else
            {
                cout << "Position (" << checkX << ", " << checkY << ") is outside the battlefield." << endl;
            }
        }
    }
}

SeeingRobot::~SeeingRobot()
{
    delete[] RobotDetectedX;
    delete[] RobotDetectedY;
}

// SteppingRobot class
SteppingRobot::SteppingRobot(string t, string n, int x, int y) : Robot(t, n, x, y)
{
    robotStepCount = 0;
}

void SteppingRobot::step(int coordinateX, int coordinateY, Battlefield &bt)
{
    if (bt.isValid(coordinateX, coordinateY) && !bt.isEmpty(coordinateX, coordinateY))
    {
        cout << getType() << " " << getName() << " steps to (" << coordinateX << ", " << coordinateY << ") and kills the enemy!" << endl;
        Robot *enemyRobot = bt.getRobotAt(coordinateX, coordinateY);
        setRobotTerminated(*enemyRobot);
        setX(coordinateX);
        setY(coordinateY);
        robotStepCount++;
    }
}

// RoboCop class
RoboCop::RoboCop(string t, string n, int x, int y) : MovingRobot(t, n, x, y), ShootingRobot(t, n, x, y), SeeingRobot(t, n, x, y)
{
}

void RoboCop::takeTurn(Battlefield &bt) // need to modify
{
    int offsetX, offsetY, targetX, targetY;
    look(0, 0, bt); // Look at current position
    move(bt);
    int count = 0;
    while (count < 3)
    {
        offsetX = rand() % 21 - 10; // Random offset between -10 and 10
        offsetY = rand() % 21 - 10;
        targetX = getX() + offsetX;
        targetY = getY() + offsetY;
        if (abs(offsetX) + abs(offsetY) <= 10 && bt.isValid(targetX, targetY))
        {
            fire(targetX, targetY, bt);
            if (getNumOfRobotTerminated() >= 3)
                setUpgradePermission(true);
            count++;
        }
    }
    resetDetection();
    if (getUpgradePermission)
        cout << getType() << " " << getName() << " has been upgraded to TerminatorRoboCop!" << endl;
}

// Terminator class
Terminator::Terminator(string t, string n, int x, int y) : MovingRobot(t, n, x, y), SeeingRobot(t, n, x, y), SteppingRobot(t, n, x, y)
{
}

void Terminator::takeTurn(Battlefield &bt)
{
    look(0, 0, bt); // Look around the 3x3 neighborhood

    int *robotCoordinateX = getRobotDetectedX();
    int *robotCoordinateY = getRobotDetectedY();

    if (getIsRobotDetected())
    {
        int targetX = robotCoordinateX[0];
        int targetY = robotCoordinateY[0];

        // Move to and terminate the enemy robot
        step(targetX, targetY, bt);
        if (getNumOfRobotTerminated() >= 3)
            setUpgradePermission(true);
        resetDetection();
        // Only step one enemy per turn
    }
    else
    {
        // No enemies in neighborhood, move randomly
        move(bt);
    }
    if (getUpgradePermission())
        cout << getType() << " " << getName() << " has been upgraded to TerminatorRoboCop!" << endl;
}

// BlueThunder class
BlueThunder::BlueThunder(string t, string n, int x, int y) : ShootingRobot(t, n, x, y)
{
    directionCount = 0;
}

void BlueThunder::takeTurn(Battlefield &bt)
{
    int targetX;
    int targetY;
    int directionIndex;

    // Define the firing directions in a clockwise sequence starting from up
    const int directions[8][2] = {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};

    while (true)
    {
        directionIndex = (directionCount) % 8;
        targetX = getX() + directions[directionIndex][0];
        targetY = getY() + directions[directionIndex][1];

        if (bt.isValid(targetX, targetY))
        {
            fire(directions[directionIndex][0], directions[directionIndex][1], bt);
            if (getNumOfRobotTerminated() >= 3)
                setUpgradePermission(true);
            directionCount++;
            break;
        }
    }
    if (getUpgradePermission())
        cout << getType() << " " << getName() << " has been upgraded to MadBot!" << endl;
}

// MadBot class
MadBot::MadBot(string t, string n, int x, int y) : BlueThunder(t, n, x, y)
{
}

void MadBot::takeTurn(Battlefield &bt)
{
    int targetX;
    int targetY;
    int directionIndex;

    const int directions[8][2] = {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};

    while (true)
    {
        directionIndex = rand() % 8; // random number 0-7
        targetX = getX() + directions[directionIndex][0];
        targetY = getY() + directions[directionIndex][1];

        if (bt.isValid(targetX, targetY))
        {
            fire(directions[directionIndex][0], directions[directionIndex][1], bt);
            if (getNumOfRobotTerminated() >= 3)
                setUpgradePermission(true);
            break;
        }
    }
    if (getUpgradePermission())
        cout << getType() << " " << getName() << " has been upgraded to RoboTank!" << endl;
}

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
    ss.str(line);
    int steps;
    while (!ss.eof())
    {
        ss >> temp;
        if (stringstream(temp) >> steps)
            break;
    }

    // Read number of robots
    getline(infile, line);
    ss.clear();
    ss.str(line);

    while (!ss.eof())
    {
        ss >> temp;
        if (stringstream(temp) >> noOfRobotPlaying)
            break;
    }

    // Read information of robots
    string tempType;
    string tempName;
    int tempX;
    int tempY;
    headRobot = nullptr;

    while (getline(infile, line))
    {
        ss.clear();
        ss.str(line);
        ss >> tempType >> tempName;
        ss >> temp;

        bool randomValueX = false;
        bool randomValueY = false;
        bool validPosition = false;
        if (!(stringstream(temp) >> tempX))
        {
            if (temp == "random")
                randomValueX = true;
            else
            {
                cout << "Invalid position!\n";
                exit(EXIT_FAILURE);
            }
        }

        ss >> temp;
        if (!(stringstream(temp) >> tempY))
        {
            if (temp == "random")
                randomValueY = true;
            else
            {
                cout << "Invalid position!\n";
                exit(EXIT_FAILURE);
            }
        }

        while ((randomValueX || randomValueY) && !validPosition)
        {
            if (randomValueX)
                tempX = rand() % w;
            else
                tempY = rand() % l;

            validPosition = true;
            if (!headRobot)
                break;
            RobotNode *robotPtr = headRobot;
            while (robotPtr != nullptr)
            {
                if (robotPtr->rb->getX() == tempX && robotPtr->rb->getY() == tempY)
                {
                    validPosition = false;
                    break;
                }
                robotPtr = robotPtr->nextRobot;
            }
        }
        initializeRobot(tempType, tempName, tempX, tempY);
    }
    infile.close();

    // Initialize waiting queue
    frontWaiting = rearWaiting = nullptr;
    noOfRobotWaiting = 0;

    // Initialize other data member
    Battlefield battlefield(w, l, headRobot);
    totalSteps = steps;
    currentStep = 0;
    totalRobots = robotsRemaining = noOfRobotPlaying;
    robotsDied = 0;
}

void War::initializeRobot(string tt, string tn, int tx, int ty)
{
    Robot *newRobot = nullptr;
    if (tt == "RoboCop")
    {
        newRobot = new RoboCop(tt, tn, tx, ty);
    }
    else if (tt == "Terminator")
    {
        newRobot = new Terminator(tt, tn, tx, ty);
    }
    else if (tt == "TerminatorRobocop")
    {
        newRobot = new TerminatorRoboCop(tt, tn, tx, ty);
    }
    else if (tt == "BlueThunder")
    {
        newRobot = new BlueThunder(tt, tn, tx, ty);
    }
    else if (tt == "MadBot")
    {
        newRobot = new MadBot(tt, tn, tx, ty);
    }
    else if (tt == "RoboTank")
    {
        newRobot = new RoboTank(tt, tn, tx, ty);
    }
    else if (tt == "UltimateRobot")
    {
        newRobot = new UltimateRobot(tt, tn, tx, ty);
    }
    else
    {
        cout << "Invalid Robot Type \"" << tt << "\"\n";
        exit(EXIT_FAILURE);
    }
    appendRobot(*newRobot);
}

void War::appendRobot(Robot &r)
{
    RobotNode *newRobot = nullptr;
    RobotNode *robotPtr = nullptr;
    newRobot = new RobotNode;
    newRobot->rb = &r;
    newRobot->nextRobot = nullptr;

    if (!headRobot)
    {
        headRobot = newRobot;
    }
    else
    {
        robotPtr = headRobot;
        while (robotPtr->nextRobot != nullptr)
            robotPtr = robotPtr->nextRobot;
        robotPtr->nextRobot = newRobot;
    }
}

void War::deleteRobot(Robot &r)
{
    RobotNode *robotPtr = nullptr;
    RobotNode *previousRobot = nullptr;

    if (!headRobot)
        return;
    if (headRobot->rb == &r)
    {
        robotPtr = headRobot;
        headRobot = headRobot->nextRobot;
        delete robotPtr;
    }
    else
    {
        robotPtr = headRobot;
        while (robotPtr != nullptr && robotPtr->rb != &r)
        {
            previousRobot = robotPtr;
            robotPtr = robotPtr->nextRobot;
        }
        if (robotPtr)
        {
            previousRobot->nextRobot = robotPtr->nextRobot;
            robotPtr = nullptr;
            delete robotPtr;
        }
    }
}

bool War::isPlayingEmpty() const
{
    return headRobot == nullptr;
}

void War::enqueueWaiting(Robot &r)
{
    RobotNode *newRobot = nullptr;

    newRobot = new RobotNode;
    newRobot->rb = &r;
    newRobot->nextRobot = nullptr;

    if (isWaitingEmpty())
    {
        frontWaiting = newRobot;
        rearWaiting = newRobot;
    }
    else
    {
        rearWaiting->nextRobot = newRobot;
        rearWaiting = newRobot;
    }

    noOfRobotWaiting++;
}

void War::dequeueWaiting(Robot &r)
{
    RobotNode *temp = nullptr;
    r = *(frontWaiting->rb);
    temp = frontWaiting;
    frontWaiting = frontWaiting->nextRobot;
    noOfRobotWaiting--;
    temp = nullptr;
    delete temp;
}

bool War::isWaitingEmpty() const
{
    return frontWaiting == nullptr;
}

Robot *War::getRobotPlaying(int i)
{
    if (i < 0 || i >= noOfRobotPlaying)
        return nullptr;
    else
    {
        RobotNode *robotPtr = headRobot;
        int count = 0;
        while (count < i)
        {
            robotPtr = robotPtr->nextRobot;
            count++;
        }
        return robotPtr->rb;
    }
}

void War::terminateRobot(Robot &r)
{
    deleteRobot(r);
    battlefield.removeRobot(r);
    r.setRemainingLives(r.getRemainingLives() - 1);
    cout << r.getType() << " " << r.getName() << " has been killed.\n";
    if (!r.stillGotLive())
    {
        cout << r.getType() << " " << r.getName() << " doesn't have any lives remained.\n";
        cout << r.getType() << " " << r.getName() << " out!!!\n";
        deleteRobot(r);
        delete &r;
    }
    else
    {
        cout << r.getType() << " " << r.getName() << "'s lives reduced by 1\n";
        cout << r.getType() << " " << r.getName() << " enter the waiting queue\n";
        enqueueWaiting(r);
    }
}

void War::promoteRobot(Robot &r)
{
    Robot *promotedRobot = nullptr;
    if (r.getType() == "RoboCop" || r.getType() == "Terminator")
    {
        promotedRobot = new TerminatorRoboCop("TerminatorRoboCop", r.getName(), r.getX(), r.getY());
    }
    else if (r.getType() == "BlueThunder")
    {
        promotedRobot = new MadBot("MadBot", r.getName(), r.getX(), r.getY());
    }
    else if (r.getType() == "MadBot")
    {
        promotedRobot = new RoboTank("RoboTank", r.getName(), r.getX(), r.getY());
    }
    else if (r.getType() == "TerminatorRoboCop" || r.getType() == "RoboTank")
    {
        promotedRobot = new UltimateRobot("UltimateRobot", r.getName(), r.getX(), r.getY());
    }
    switchRobot(r, *promotedRobot);
    delete &r;
}

void War::switchRobot(Robot &r1, Robot &r2)
{
    RobotNode *currentPtr = headRobot;
    while (currentPtr != nullptr)
    {
        if (currentPtr->rb == &r1)
        {
            currentPtr->rb = &r2;
            break;
        }
        currentPtr = currentPtr->nextRobot;
    }
}

void War::startWar()
{
    while (currentStep < totalSteps && robotsRemaining > 0)
    {
        cout << "Current step: " << currentStep << endl
             << endl;
        battlefield.displayField();
        RobotNode *currentPtr = headRobot;
        while (currentPtr != nullptr && !isPlayingEmpty())
        {
            cout << "Action:\n";
            Robot *currentRobot = currentPtr->rb;
            currentRobot->takeTurn(battlefield);

            cout << "Result:\n";
            for (int i = 0; i < 3; i++)
            {
                if (&currentRobot->getRobotTerminated(i) == nullptr)
                    break;
                else
                    terminateRobot(currentRobot->getRobotTerminated(i));
            }
            if (currentRobot->getUpgradePermission())
                promoteRobot(*currentRobot);

            currentPtr = currentPtr->nextRobot;
        }
        while (!isWaitingEmpty() && robotsRemaining > 0)
        {
            Robot *returnRobot;
            dequeueWaiting(*returnRobot);
            appendRobot(*returnRobot);
        }
    }
    if (robotsRemaining <= 0)
    {
        cout << "All robots have been terminated!\n Game Over!!!\n";
    }
    else if (currentStep >= totalSteps && robotsRemaining > 1)
    {
        cout << "The war has reached the maximum number of steps.\nDraw!!!\n";
    }
    else
    {
        cout << "War end.\nThe winner is " << headRobot->rb->getName() << "!!!\n Congratulations!!!\n";
    }
}

War::~War()
{
    // Clean up the battlefield
    delete &battlefield;

    // Clean up the robots in the playing list
    RobotNode *currentRobot = headRobot;
    while (currentRobot)
    {
        RobotNode *temp = currentRobot;
        currentRobot = currentRobot->nextRobot;
        delete temp->rb;
        delete temp;
    }

    // Clean up the robots in the waiting queue
    while (!isWaitingEmpty())
    {
        Robot *dummy;
        dequeueWaiting((*dummy));
        delete dummy;
    }
}
