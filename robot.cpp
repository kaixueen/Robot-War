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

void Robot::setRobotTerminated(Robot &r)
{
    robotTerminated[numOfRobotTerminated] = &r;
    numOfRobotTerminated++;
}

void Robot::resetRobotTerminated()
{
    numOfRobotTerminated = 0;
    for (int i = 0; i < 3; i++)
        robotTerminated[i] = nullptr;
}

Robot::~Robot()
{
    for (int i = 0; i < 3; i++)
        delete robotTerminated[i];
}

// RobotNode class
RobotNode::RobotNode(RobotNode &&rn)
{
    robot = rn.robot;
    next = rn.next;
    rn.robot = nullptr;
    rn.next = nullptr;
}

RobotNode &RobotNode::operator=(const RobotNode &rn) // copy assignment operator
{
    if (this != &rn)
    {
        robot = rn.robot;
        next = rn.next;
    }
    return *this;
}

// RobotList class
RobotList::RobotList(const RobotList &rl) // copy constructor
{
    RobotNode *origListPtr = rl.headPtr;

    if (origListPtr == nullptr)
        headPtr = nullptr;
    else
    {
        headPtr = new RobotNode();
        headPtr->setRobot(origListPtr->getRobot()); // copy first node

        // copy remaining nodes
        RobotNode *newListPtr = headPtr;
        origListPtr = origListPtr->getNext();
        while (origListPtr != nullptr)
        {
            Robot *nextRobot = &origListPtr->getRobot();
            RobotNode *newNodePtr = new RobotNode(*nextRobot);
            newListPtr->setNext(newNodePtr);
            newListPtr = newListPtr->getNext();
        }
        newListPtr->setNext(nullptr);
    }
}

RobotList::RobotList(RobotList &&rl)
{
    headPtr = rl.headPtr;
    robotCount = rl.robotCount;
    rl.headPtr = nullptr;
    rl.robotCount = 0;
}

RobotList &RobotList::operator=(const RobotList &rl)
{
    if (this != &rl)
    {
        while (headPtr) // clear existing content
        {
            RobotNode *nodePtr = headPtr;
            headPtr = headPtr->getNext();
            delete nodePtr;
        }
        robotCount = 0;

        // copy new list
        RobotNode *origListPtr = rl.headPtr;

        if (origListPtr == nullptr)
            headPtr = nullptr;
        else
        {
            headPtr = new RobotNode();
            headPtr->setRobot(origListPtr->getRobot()); // copy first node

            // copy remaining nodes
            RobotNode *newListPtr = headPtr;
            origListPtr = origListPtr->getNext();
            while (origListPtr != nullptr)
            {
                Robot *nextRobot = &origListPtr->getRobot();
                RobotNode *newNodePtr = new RobotNode(*nextRobot);
                newListPtr->setNext(newNodePtr);
                newListPtr = newListPtr->getNext();
            }
            newListPtr->setNext(nullptr);
        }
    }
    return *this;
}

RobotList::~RobotList()
{
    RobotNode *nodePtr;
    RobotNode *nextNode;
    nodePtr = headPtr;

    while (nodePtr != nullptr)
    {
        nextNode = nodePtr->getNext();
        delete nodePtr;
        nodePtr = nextNode;
    }
}

RobotNode *RobotList::getNodeAt(int position)
{
    if (position >= 0 && position < robotCount)
    {
        RobotNode *nodePtr = headPtr;
        for (int skip = 0; skip < position; skip++)
        {
            nodePtr = nodePtr->getNext();
        }
        return nodePtr;
    }
}

void RobotList::appendRobot(Robot &rb)
{
    RobotNode *newNode; // To point to a new node
    RobotNode *nodePtr; // To move through the list

    newNode = new RobotNode(rb);

    if (!headPtr)
        headPtr = newNode;
    else
    {
        nodePtr = headPtr;
        while (nodePtr->getNext())
            nodePtr = nodePtr->getNext();
        nodePtr->setNext(newNode);
    }
    robotCount++;
}

bool RobotList::removeRobot(Robot &rb)
{
    RobotNode *nodePtr;  // To traverse the list
    RobotNode *prevNode; // To point to the previous node

    if (!headPtr)
        return false;

    if (&headPtr->getRobot() == &rb)
    {
        nodePtr = headPtr->getNext();
        headPtr = nodePtr;
        robotCount--;
        return true;
    }
    else
    {
        nodePtr = headPtr;
        while (nodePtr != nullptr && &nodePtr->getRobot() != &rb)
        {
            prevNode = nodePtr;
            nodePtr = nodePtr->getNext();
        }
        if (nodePtr)
        {
            prevNode->setNext(nodePtr->getNext());
            robotCount--;
            return true;
        }
        return false;
    }
}

bool RobotList::replaceRobot(Robot &oldRobot, Robot &newRobot) // replace war replaceRobot function
{
    RobotNode *nodePtr;
    RobotNode *prevNode;
    RobotNode *newNode = new RobotNode(newRobot);

    if (!headPtr)
        return false;

    if (&headPtr->getRobot() == &oldRobot)
    {
        nodePtr = headPtr->getNext();
        delete headPtr;
        headPtr = newNode;
        newNode->setNext(nodePtr);
        return true;
    }
    else
    {
        nodePtr = headPtr;
        while (nodePtr != nullptr && &nodePtr->getRobot() != &oldRobot)
        {
            prevNode = nodePtr;
            nodePtr = nodePtr->getNext();
        }
        if (nodePtr)
        {
            prevNode->setNext(newNode);
            newNode->setNext(nodePtr->getNext());
            delete nodePtr;
            return true;
        }
    }
    return false;
}

void RobotList::displayList() const // testing function
{
    RobotNode *nodePtr;
    nodePtr = headPtr;
    while (nodePtr)
    {
        Robot *rb = &nodePtr->getRobot();
        cout << rb->getType() << " " << rb->getName() << endl;
        nodePtr = nodePtr->getNext();
    }
}

// Battlefield class
Battlefield::Battlefield()
{
    width = 0;
    length = 0;
    emptyCell = robotCell = boundary = ' ';
    cellArr = nullptr;
}

Battlefield::Battlefield(int w, int l, RobotList *rl)
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

    if (rl->getListLength() == 0)
        return;

    int tempX, tempY;
    Robot *tempR;
    for (int i = 0; i < rl->getListLength(); i++)
    {
        tempR = &rl->getNodeAt(i)->getRobot();
        tempY = tempR->getY();
        tempX = tempR->getX();
        if (isValid(tempX, tempY) && isEmpty(tempX, tempY))
            cellArr[tempY][tempX] = tempR;
    }
    tempR = nullptr;
    delete tempR;
}

Battlefield &Battlefield::operator=(const Battlefield &right)
{
    if (this != &right)
    {
        for (int i = 0; i < length; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                delete cellArr[i][j];
            }
            delete[] cellArr[i];
        }
        delete[] cellArr;

        width = right.width;
        length = right.length;
        emptyCell = right.emptyCell;
        robotCell = right.robotCell;
        boundary = right.boundary;

        cellArr = new Robot **[length];

        for (int i = 0; i < length; i++)
        {
            cellArr[i] = new Robot *[width];
            for (int j = 0; j < width; j++)
            {
                cellArr[i][j] = nullptr;
                if (right.cellArr[i][j] != nullptr)
                    cellArr[i][j] = right.cellArr[i][j];
            }
        }
    }
    return *this;
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

Robot *Battlefield::getRobotAt(int x, int y)
{
    if (isValid(x, y) && !isEmpty(x, y))
        return cellArr[y][x];
    return nullptr;
}

Battlefield::~Battlefield()
{
    for (int i = 0; i < length; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            delete cellArr[i][j];
        }
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

            if (checkX != getX() && checkY != getY())
            {
                if (bt.isValid(checkX, checkY))
                {
                    if (!bt.isEmpty(checkX, checkY))
                    {
                        cout << "Position (" << checkX << ", " << checkY;
                        cout << ") contains an enemy robot." << endl;
                        isRobotDetected = true;
                        RobotDetectedX[numOfRobotDetected] = checkX;
                        RobotDetectedY[numOfRobotDetected] = checkY;
                        numOfRobotDetected++;
                    }
                }
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
RoboCop::RoboCop(string t, string n, int x, int y) : MovingRobot(t, n, x, y), ShootingRobot(t, n, x, y), SeeingRobot(t, n, x, y), Robot(t, n, x, y)
{
}

void RoboCop::takeTurn(Battlefield &bt)
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
            fire(offsetX, offsetY, bt);
            if (getNumOfRobotTerminated() >= 3)
                setUpgradePermission(true);
            count++;
        }
    }
    resetDetection();
}

// Terminator class
Terminator::Terminator(string t, string n, int x, int y) : MovingRobot(t, n, x, y), SeeingRobot(t, n, x, y), SteppingRobot(t, n, x, y), Robot(t, n, x, y)
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
}

// BlueThunder class
BlueThunder::BlueThunder(string t, string n, int x, int y) : ShootingRobot(t, n, x, y), Robot(t, n, x, y)
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
}

// Madbot class
Madbot::Madbot(string t, string n, int x, int y) : BlueThunder(t, n, x, y), ShootingRobot(t, n, x, y), Robot(t, n, x, y)
{
}

void Madbot::takeTurn(Battlefield &bt)
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
}

// TerminatorRoboCop class
TerminatorRoboCop::TerminatorRoboCop(string t, string n, int x, int y) : RoboCop(t, n, x, y), Terminator(t, n, x, y), MovingRobot(t, n, x, y), SeeingRobot(t, n, x, y), SteppingRobot(t, n, x, y), Robot(t, n, x, y)
{
}

void TerminatorRoboCop::takeTurn(Battlefield &bt)
{
    Terminator::takeTurn(bt);
    int offsetX, offsetY, targetX, targetY;
    int count = 0;
    while (count < 3)
    {
        offsetX = rand() % 21 - 10; // Random offset between -10 and 10
        offsetY = rand() % 21 - 10;
        targetX = getX() + offsetX;
        targetY = getY() + offsetY;
        if (abs(offsetX) + abs(offsetY) <= 10 && bt.isValid(targetX, targetY))
        {
            fire(offsetX, offsetY, bt);
            if (getNumOfRobotTerminated() >= 3)
                setUpgradePermission(true);
            count++;
        }
    }
}
// RoboTank class
RoboTank::RoboTank(string t, string n, int x, int y) : Madbot(t, n, x, y), BlueThunder(t, n, x, y), ShootingRobot(t, n, x, y), Robot(t, n, x, y)
{
}

void RoboTank::takeTurn(Battlefield &bt)
{
    int offsetX, offsetY, targetX, targetY;

    while (true)
    {
        offsetX = rand();
        offsetY = rand();
        targetX = getX() + offsetX;
        targetY = getY() + offsetY;
        if (bt.isValid(targetX, targetY))
        {
            fire(offsetX, offsetY, bt);
            if (getNumOfRobotTerminated() >= 3)
                setUpgradePermission(true);
            break;
        }
    }
}

// UltimateRobot class
UltimateRobot::UltimateRobot(string t, string n, int x, int y) : TerminatorRoboCop(t, n, x, y), RoboTank(t, n, x, y), RoboCop(t, n, x, y), Terminator(t, n, x, y), Madbot(t, n, x, y), BlueThunder(t, n, x, y), MovingRobot(t, n, x, y), SeeingRobot(t, n, x, y), SteppingRobot(t, n, x, y), Robot(t, n, x, y)
{
}

void UltimateRobot::takeTurn(Battlefield &bt)
{
    Terminator::takeTurn(bt);
    for (int i = 0; i < 3; i++)
    {
        RoboTank::takeTurn(bt);
    }
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
            if (randomValueY)
                tempY = rand() % l;

            validPosition = true;
            if (robotPlaying->getListLength() == 0)
                break;
            for (int i = 0; i < robotPlaying->getListLength(); i++)
            {
                if (robotPlaying->getNodeAt(i)->getRobot().getX() == tempX && robotPlaying->getNodeAt(i)->getRobot().getY() == tempY)
                {
                    validPosition = false;
                    break;
                }
            }
        }
        initializeRobot(tempType, tempName, tempX, tempY);
    }
    infile.close();

    // Initialize waiting queue
    frontWaiting = rearWaiting = nullptr;
    noOfRobotWaiting = 0;

    // Initialize other data member
    battlefield = Battlefield(w, l, robotPlaying);
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
    else if (tt == "TerminatorRoboCop")
    {
        newRobot = new TerminatorRoboCop(tt, tn, tx, ty);
    }
    else if (tt == "BlueThunder")
    {
        newRobot = new BlueThunder(tt, tn, tx, ty);
    }
    else if (tt == "Madbot")
    {
        newRobot = new Madbot(tt, tn, tx, ty);
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
    robotPlaying->appendRobot(*newRobot);
}

bool War::isPlayingEmpty() const
{
    return robotPlaying->getListLength() == 0;
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
    noOfRobotPlaying++;
    noOfRobotWaiting--;
    temp = nullptr;
    delete temp;
}

bool War::isWaitingEmpty() const
{
    return frontWaiting == nullptr;
}

void War::terminateRobot(Robot &r)
{
    robotPlaying->removeRobot(r); // remove from linked list
    battlefield.removeRobot(r);   // remove from battlefield

    r.setRemainingLives(r.getRemainingLives() - 1); // reduce remaining lives
    cout << r.getType() << " " << r.getName() << " has been killed.\n";
    noOfRobotPlaying--;

    if (!r.stillGotLive()) // if no lives remained, the robot is game over
    {
        cout << r.getType() << " " << r.getName() << " doesn't have any lives remained.\n";
        cout << r.getType() << " " << r.getName() << " out!!!\n";
        robotsRemaining--;
        robotsDied++;
        delete &r;
    }
    else // if got lives remained, enter waiting queue
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
        cout << r.getType() << " " << r.getName() << " has been upgraded to TerminatorRoboCop!" << endl;
        promotedRobot = new TerminatorRoboCop("TerminatorRoboCop", r.getName(), r.getX(), r.getY());
    }
    else if (r.getType() == "BlueThunder")
    {
        cout << r.getType() << " " << r.getName() << " has been upgraded to MadBot!" << endl;
        promotedRobot = new Madbot("Madbot", r.getName(), r.getX(), r.getY());
    }
    else if (r.getType() == "Madbot")
    {
        cout << r.getType() << " " << r.getName() << " has been upgraded to RoboTank!" << endl;
        promotedRobot = new RoboTank("RoboTank", r.getName(), r.getX(), r.getY());
    }
    else if (r.getType() == "TerminatorRoboCop" || r.getType() == "RoboTank")
    {
        cout << r.getType() << " " << r.getName() << " has been upgraded to UltimateRobot!" << endl;
        promotedRobot = new UltimateRobot("UltimateRobot", r.getName(), r.getX(), r.getY());
    }
    robotPlaying->replaceRobot(r, *promotedRobot);
    battlefield.removeRobot(r);
    battlefield.updatePosition(promotedRobot, promotedRobot->getX(), promotedRobot->getY());
    delete &r;
}

void War::startWar()
{

    while (currentStep < totalSteps && robotsRemaining > 0)
    {
        currentStep++;
        cout << "Current step: " << currentStep << endl
             << endl;
        battlefield.displayField();

        for (int i = 0; i < robotPlaying->getListLength(); i++)
        {
            cout << "Action:\n";
            Robot *currentRobot = &robotPlaying->getNodeAt(i)->getRobot();
            int prevX = currentRobot->getX();
            int prevY = currentRobot->getY();
            currentRobot->takeTurn(battlefield);
            battlefield.removeRobot(prevX, prevY);
            battlefield.updatePosition(currentRobot, currentRobot->getX(), currentRobot->getY());

            for (int i = 0; i < 3; i++)
            {
                if (&currentRobot->getRobotTerminated(i) == nullptr)
                    break;
                else
                {
                    terminateRobot(currentRobot->getRobotTerminated(i));
                }
            }
            if (currentRobot->getUpgradePermission() && currentRobot->getType() != "UltimateRobot")
                promoteRobot(*currentRobot);
        }
        if (!isWaitingEmpty() && robotsRemaining > 0)
        {
            Robot *returnRobot;
            dequeueWaiting(*returnRobot);
            robotPlaying->appendRobot(*returnRobot);
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
        cout << "War end.\nThe winner is " << robotPlaying->getNodeAt(0)->getRobot().getName() << "!!!\n Congratulations!!!\n";
    }
}

War::~War()
{
    // Clean up the battlefield
    delete &battlefield;

    // Clean up the robots in the playing list
    delete robotPlaying;

    // Clean up the robots in the waiting queue
    while (!isWaitingEmpty())
    {
        Robot *dummy;
        dequeueWaiting((*dummy));
        delete dummy;
    }
}
