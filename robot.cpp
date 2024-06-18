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
    robotSymbol = 'R';
    robotPositionX = robotPositionY = -1;
    remainingLives = 3;
    upgradePermission = false;
    numOfRobotTerminated = 0;
    currentNumOfRobotTerminated = 0;
    for (int i = 0; i < 3; i++)
        robotTerminated[i] = nullptr;
}

Robot::Robot(string t, string n, int x, int y, char s) // Parameterized constructor
{
    robotName = n;
    robotType = t;
    robotSymbol = s;
    robotPositionX = x;
    robotPositionY = y;
    remainingLives = 3;
    upgradePermission = false;
    numOfRobotTerminated = 0;
    currentNumOfRobotTerminated = 0;
    for (int i = 0; i < 3; i++)
        robotTerminated[i] = nullptr;
}

Robot::Robot(const Robot &r) // Copy constructor
{
    robotName = r.robotName;
    robotType = r.robotType;
    robotSymbol = r.robotSymbol;
    robotPositionX = r.robotPositionX;
    robotPositionY = r.robotPositionY;
    remainingLives = r.remainingLives;
    upgradePermission = r.upgradePermission;
    numOfRobotTerminated = r.numOfRobotTerminated;
    currentNumOfRobotTerminated = r.currentNumOfRobotTerminated;
    for (int i = 0; i < 3; i++)
        robotTerminated[i] = r.robotTerminated[i];
}

Robot::Robot(Robot &&r) noexcept : robotName(r.robotName), robotType(r.robotType), robotSymbol(r.robotSymbol), robotPositionX(r.robotPositionX), robotPositionY(r.robotPositionY), remainingLives(r.remainingLives), upgradePermission(r.upgradePermission), numOfRobotTerminated(r.numOfRobotTerminated), currentNumOfRobotTerminated(r.currentNumOfRobotTerminated) // move constructor
{
    // Transfer ownership of terminated robots
    for (int i = 0; i < 3; ++i)
    {
        robotTerminated[i] = r.robotTerminated[i];
        r.robotTerminated[i] = nullptr;
    }
    r.numOfRobotTerminated = 0; // Reset the number of terminated robots
    r.currentNumOfRobotTerminated = 0;
}

Robot &Robot::operator=(const Robot &right) // Assignment operator overloading
{
    if (this != &right)
    {
        robotName = right.robotName;
        robotType = right.robotType;
        robotSymbol = right.robotSymbol;
        robotPositionX = right.robotPositionX;
        robotPositionY = right.robotPositionY;
        remainingLives = right.remainingLives;
        upgradePermission = right.upgradePermission;
        numOfRobotTerminated = right.numOfRobotTerminated;
        currentNumOfRobotTerminated = right.currentNumOfRobotTerminated;
        for (int i = 0; i < 3; i++)
            robotTerminated[i] = right.robotTerminated[i];
    }
    return *this;
}

void Robot::setRobotTerminated(Robot &r)
{
    robotTerminated[currentNumOfRobotTerminated] = &r;
    numOfRobotTerminated++;
    currentNumOfRobotTerminated++;
}

void Robot::resetRobotTerminated()
{
    for (int i = 0; i < 3; i++)
        robotTerminated[i] = nullptr;
    resetCurrentNumOfRobotTerminated();
}

Robot::~Robot()
{
    for (int i = 0; i < 3; i++)
        robotTerminated[i] = nullptr;
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
        delete robot;
        delete next;
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
            Robot *nextRobot = origListPtr->getRobot();
            RobotNode *newNodePtr = new RobotNode(nextRobot);
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
                Robot *nextRobot = origListPtr->getRobot();
                RobotNode *newNodePtr = new RobotNode(nextRobot);
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
    return nullptr;
}

void RobotList::appendRobot(Robot *rb)
{
    RobotNode *newNode = new RobotNode(rb); // To point to a new node

    if (headPtr == nullptr)
        headPtr = newNode;
    else
    {
        RobotNode *nodePtr = headPtr;
        while (nodePtr->getNext())
            nodePtr = nodePtr->getNext();
        nodePtr->setNext(newNode);
        nodePtr = nullptr;
    }
    robotCount++;
    newNode = nullptr;
}

bool RobotList::removeRobot(Robot &rb)
{
    RobotNode *nodePtr;  // To traverse the list
    RobotNode *prevNode; // To point to the previous node

    if (!headPtr)
        return false;

    if (headPtr->getRobot() == &rb)
    {
        // headPtr = headPtr->getNext();
        // headPtr = nodePtr;
        nodePtr = headPtr;
        headPtr = headPtr->getNext();
        nodePtr->setRobot(nullptr);
        nodePtr->setNext(nullptr);
        robotCount--;
        return true;
    }
    else
    {
        nodePtr = headPtr;
        while (nodePtr != nullptr && nodePtr->getRobot() != &rb)
        {
            prevNode = nodePtr;
            nodePtr = nodePtr->getNext();
        }
        if (nodePtr)
        {
            prevNode->setNext(nodePtr->getNext());
            robotCount--;
            return true;
            nodePtr->setRobot(nullptr);
            nodePtr->setNext(nullptr);
        }
        return false;
    }
}

bool RobotList::replaceRobot(Robot *oldRobot, Robot *newRobot) // replace war replaceRobot function
{
    RobotNode *nodePtr;
    RobotNode *prevNode;
    RobotNode *newNode = new RobotNode(newRobot);

    if (!headPtr)
        return false;

    if (headPtr->getRobot() == oldRobot)
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
        while (nodePtr != nullptr && nodePtr->getRobot() != oldRobot)
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
        Robot *rb = nodePtr->getRobot();
        cout << rb->getType() << " " << rb->getName() << endl;
        nodePtr = nodePtr->getNext();
    }
}

// RobotQueue class
RobotQueue::RobotQueue()
{
    front = nullptr;
    rear = nullptr;
    numRobots = 0;
}

// Copy constructor
RobotQueue::RobotQueue(const RobotQueue &rq)
{
    RobotNode *current = rq.front;
    while (current != nullptr)
    {
        enqueue(current->getRobot());
        current = current->getNext();
    }
}

// Move constructor
RobotQueue::RobotQueue(RobotQueue &&rq)
{
    front = rq.front;
    rear = rq.rear;
    numRobots = rq.numRobots;
    rq.front = nullptr;
    rq.rear = nullptr;
    rq.numRobots = 0;
}

// Copy assignment operator
RobotQueue &RobotQueue::operator=(const RobotQueue &rq)
{
    if (this != &rq)
    {
        clear();
        RobotNode *current = rq.front;
        while (current != nullptr)
        {
            enqueue(current->getRobot());
            current = current->getNext();
        }
    }
    return *this;
}

RobotQueue::~RobotQueue()
{
    clear();
}

void RobotQueue::enqueue(Robot *rb)
{
    RobotNode *newNode = new RobotNode(rb);
    if (isEmpty())
    {
        front = rear = newNode;
    }
    else
    {
        rear->setNext(newNode);
        rear = newNode;
    }
    newNode = nullptr;
    delete newNode;
    numRobots++;
}

void RobotQueue::dequeue(Robot *&rb)
{
    if (isEmpty())
    {
        cout << "The queue is empty.\n";
    }
    else
    {
        RobotNode *temp = nullptr;
        rb = front->getRobot();
        temp = front;

        if (front == rear)
        { // Special case: one node in queue
            front = nullptr;
            rear = nullptr;
        }
        else
        {
            front = front->getNext();
        }

        temp->setRobot(nullptr);
        temp->setNext(nullptr);
        numRobots--;
    }
}

bool RobotQueue::isEmpty() const
{
    bool status;

    if (numRobots > 0)
        status = false;
    else
        status = true;
    return status;
}

void RobotQueue::clear()
{
    Robot *rb;

    while (!isEmpty())
        dequeue(rb);
}

// Battlefield class
Battlefield::Battlefield()
{
    width = 0;
    length = 0;
    emptyCell = boundary = ' ';
    cellArr = nullptr;
}

Battlefield::Battlefield(int w, int l, RobotList &rl)
{
    width = w;
    length = l;
    emptyCell = ' ';
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

    if (rl.getListLength() == 0)
        return;

    int tempX, tempY;
    Robot *tempR = nullptr;
    for (int i = 0; i < rl.getListLength(); i++)
    {
        tempR = rl.getNodeAt(i)->getRobot();

        tempY = tempR->getY();
        tempX = tempR->getX();
        if (isValid(tempX, tempY) && isEmpty(tempX, tempY))
            cellArr[tempY][tempX] = tempR;
    }
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
        boundary = right.boundary;

        cellArr = new Robot **[length];

        for (int i = 0; i < length; i++)
        {
            cellArr[i] = new Robot *[width];
            for (int j = 0; j < width; j++)
            {
                cellArr[i][j] = nullptr;
                if (right.cellArr[i][j] != nullptr)
                {
                    cellArr[i][j] = right.cellArr[i][j];
                }
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
                cout << cellArr[i][j]->getSymbol();

            if (j == width - 1)
                cout << boundary << endl;
        }
    }

    for (int i = 0; i < width + 2; i++)
        cout << boundary;
    cout << endl;
}

void Battlefield::displayField(ofstream &outfile) const
{
    for (int i = 0; i < width + 2; i++)
        outfile << boundary;
    outfile << endl;

    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0)
                outfile << boundary;

            if (cellArr[i][j] == nullptr)
                outfile << emptyCell;
            else
                outfile << cellArr[i][j]->getSymbol();

            if (j == width - 1)
                outfile << boundary << endl;
        }
    }

    for (int i = 0; i < width + 2; i++)
        outfile << boundary;
    outfile << endl;
}

bool Battlefield::updatePosition(Robot *r, int x, int y)
{
    if (r == nullptr)
        return false;

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
            cellArr[i][j] = nullptr;
        }
        delete[] cellArr[i];
    }
    delete[] cellArr;
}

// MovingRobot class
MovingRobot::MovingRobot(string n, string t, int x, int y, char s) : Robot(n, t, x, y, s)
{
}

void MovingRobot::move(Battlefield &bt, ofstream &outfile)
{
    int prevX, prevY, newX, newY;
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
            prevX = getX();
            prevY = getY();
            bt.removeRobot(prevX, prevY);
            setX(newX);
            setY(newY);
            bt.updatePosition(this, getX(), getY());

            invalidMove = false;
        }
    }
    cout << getType() << " " << getName() << " move to (" << newX << ", " << newY << ").\n";
    outfile << getType() << " " << getName() << " move to (" << newX << ", " << newY << ").\n";
}

// ShootingRobot class
ShootingRobot::ShootingRobot(string t, string n, int x, int y, char s) : Robot(t, n, x, y, s)
{
}

void ShootingRobot::fire(int offsetX, int offsetY, Battlefield &bt, ofstream &outfile)
{
    if (offsetX == 0 && offsetY == 0)
        return;
    int targetX = getX() + offsetX;
    int targetY = getY() + offsetY;

    if (bt.isValid(targetX, targetY))
    {
        if (!bt.isEmpty(targetX, targetY))
        {
            Robot *enemyRobot = bt.getRobotAt(targetX, targetY);
            cout << getType() << " " << getName() << " fires at (" << targetX << ", " << targetY << ") and destroys " << enemyRobot->getType() << " " << enemyRobot->getName() << "!" << endl;
            outfile << getType() << " " << getName() << " fires at (" << targetX << ", " << targetY << ") and destroys " << enemyRobot->getType() << " " << enemyRobot->getName() << "!" << endl;
            setRobotTerminated(*enemyRobot);
        }
        else
        {
            cout << getType() << " " << getName() << " fires at (" << targetX << ", " << targetY << ") but the position is empty." << endl;
            outfile << getType() << " " << getName() << " fires at (" << targetX << ", " << targetY << ") but the position is empty." << endl;
        }
    }
}

// SeeingRobot class
SeeingRobot::SeeingRobot(string t, string n, int x, int y, char s) : Robot(t, n, x, y, s)
{
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

void SeeingRobot::look(int offsetX, int offsetY, Battlefield &bt, ofstream &outfile) // need to reset the robot detected every turn, does it look at the offset position, if not only immediate neighbourhood
{

    int centerX = getX() + offsetX;
    int centerY = getY() + offsetY;

    cout << getType() << " " << getName() << " is looking around:" << endl;
    outfile << getType() << " " << getName() << " is looking around:" << endl;
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
                        cout << "Position (" << checkX << ", " << checkY << ") contains an enemy robot." << endl;
                        outfile << "Position (" << checkX << ", " << checkY << ") contains an enemy robot." << endl;
                        RobotDetectedX[numOfRobotDetected] = checkX;
                        RobotDetectedY[numOfRobotDetected] = checkY;
                        numOfRobotDetected++;
                    }
                }
            }
        }
    }
    if (numOfRobotDetected == 0)
    {
        cout << "No robot has been detected by " << getType() << " " << getName() << " this round.\n";
        outfile << "No robot has been detected by " << getType() << " " << getName() << " this round.\n";
        ;
    }
}

SeeingRobot::~SeeingRobot()
{
    delete[] RobotDetectedX;
    delete[] RobotDetectedY;
}

// SteppingRobot class
SteppingRobot::SteppingRobot(string t, string n, int x, int y, char s) : Robot(t, n, x, y, s)
{
    robotStepCount = 0;
}

void SteppingRobot::step(int coordinateX, int coordinateY, Battlefield &bt, ofstream &outfile)
{
    if (bt.isValid(coordinateX, coordinateY) && !bt.isEmpty(coordinateX, coordinateY))
    {
        Robot *enemyRobot = bt.getRobotAt(coordinateX, coordinateY);
        cout << getType() << " " << getName() << " steps to (" << coordinateX << ", " << coordinateY << ") and kills " << enemyRobot->getType() << " " << enemyRobot->getName() << "!\n";
        outfile << getType() << " " << getName() << " steps to (" << coordinateX << ", " << coordinateY << ") and kills " << enemyRobot->getType() << " " << enemyRobot->getName() << "!\n";

        setRobotTerminated(*enemyRobot);
        setX(coordinateX);
        setY(coordinateY);
        robotStepCount++;
    }
    else
    {
        cout << "No robot has been stepped by " << getType() << " " << getName() << " this round.\n";
        outfile << "No robot has been stepped by " << getType() << " " << getName() << " this round.\n";
    }
}

// RoboCop class
RoboCop::RoboCop(string t, string n, int x, int y, char s) : MovingRobot(t, n, x, y, s), ShootingRobot(t, n, x, y, s), SeeingRobot(t, n, x, y, s), Robot(t, n, x, y, s)
{
}

void RoboCop::takeTurn(Battlefield &bt, ofstream &outfile)
{
    int offsetX, offsetY, targetX, targetY;

    look(0, 0, bt, outfile); // Look at current position

    move(bt, outfile);
    int count = 0;
    while (count < 3)
    {
        offsetX = rand() % 21 - 10; // Random offset between -10 and 10
        offsetY = rand() % 21 - 10;
        targetX = getX() + offsetX;
        targetY = getY() + offsetY;
        if (abs(offsetX) + abs(offsetY) <= 10 && bt.isValid(targetX, targetY))
        {
            fire(offsetX, offsetY, bt, outfile);
            if (getNumOfRobotTerminated() >= 3)
                setUpgradePermission(true);
            count++;
        }
    }
    resetDetection();
}

// Terminator class
Terminator::Terminator(string t, string n, int x, int y, char s) : MovingRobot(t, n, x, y, s), SeeingRobot(t, n, x, y, s), SteppingRobot(t, n, x, y, s), Robot(t, n, x, y, s)
{
}

void Terminator::takeTurn(Battlefield &bt, ofstream &outfile)
{
    look(0, 0, bt, outfile); // Look around the 3x3 neighborhood

    int *robotCoordinateX = getRobotDetectedX();
    int *robotCoordinateY = getRobotDetectedY();

    if (getIsRobotDetected())
    {
        int targetX = robotCoordinateX[0];
        int targetY = robotCoordinateY[0];

        // Move to and terminate the enemy robot
        step(targetX, targetY, bt, outfile);
        if (getNumOfRobotTerminated() >= 3)
            setUpgradePermission(true);
        resetDetection();
        // Only step one enemy per turn
    }
    else
    {
        // No enemies in neighborhood, move randomly
        move(bt, outfile);
    }
}

// BlueThunder class
BlueThunder::BlueThunder(string t, string n, int x, int y, char s) : ShootingRobot(t, n, x, y, s), Robot(t, n, x, y, s)
{
    directionCount = 0;
}

void BlueThunder::takeTurn(Battlefield &bt, ofstream &outfile)
{
    int targetX;
    int targetY;
    int directionIndex;
    int attempts = 0;
    const int maxAttempts = 8; // Maximum attempts to find a valid direction

    // Define the firing directions in a clockwise sequence starting from up
    const int directions[8][2] = {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};

    while (attempts < maxAttempts)
    {
        directionIndex = (directionCount + attempts) % 8;
        targetX = getX() + directions[directionIndex][0];
        targetY = getY() + directions[directionIndex][1];

        if (bt.isValid(targetX, targetY))
        {
            fire(directions[directionIndex][0], directions[directionIndex][1], bt, outfile);
            if (getNumOfRobotTerminated() >= 3)
                setUpgradePermission(true);
            directionCount = (directionCount + 1) % 8; // Update directionCount for the next turn
            break;
        }
        attempts++;
    }
}

// Madbot class
Madbot::Madbot(string t, string n, int x, int y, char s) : BlueThunder(t, n, x, y, s), ShootingRobot(t, n, x, y, s), Robot(t, n, x, y, s)
{
}

void Madbot::takeTurn(Battlefield &bt, ofstream &outfile)
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
            fire(directions[directionIndex][0], directions[directionIndex][1], bt, outfile);
            if (getNumOfRobotTerminated() >= 3)
                setUpgradePermission(true);
            break;
        }
    }
}

// TerminatorRoboCop class
TerminatorRoboCop::TerminatorRoboCop(string t, string n, int x, int y, char s) : RoboCop(t, n, x, y, s), Terminator(t, n, x, y, s), MovingRobot(t, n, x, y, s), SeeingRobot(t, n, x, y, s), SteppingRobot(t, n, x, y, s), Robot(t, n, x, y, s)
{
}

void TerminatorRoboCop::takeTurn(Battlefield &bt, ofstream &outfile)
{
    Terminator::takeTurn(bt, outfile);
    int offsetX, offsetY, targetX, targetY;
    int count = 0;
    while (count < 3)
    {
        offsetX = rand() % 21 - 10; // Random offset between -10 and 10
        offsetY = rand() % 21 - 10;
        if (offsetX == 0 && offsetY == 0)
            continue;
        targetX = getX() + offsetX;
        targetY = getY() + offsetY;
        if (abs(offsetX) + abs(offsetY) <= 10 && bt.isValid(targetX, targetY))
        {
            fire(offsetX, offsetY, bt, outfile);
            if (getNumOfRobotTerminated() >= 3)
                setUpgradePermission(true);
            count++;
        }
    }
}
// RoboTank class
RoboTank::RoboTank(string t, string n, int x, int y, char s) : Madbot(t, n, x, y, s), BlueThunder(t, n, x, y, s), ShootingRobot(t, n, x, y, s), Robot(t, n, x, y, s)
{
}

void RoboTank::takeTurn(Battlefield &bt, ofstream &outfile)
{
    int offsetX, offsetY, targetX, targetY;
    while (true)
    {
        // Generate offsets in the range [-width/2, width/2] and [-length/2, length/2]
        offsetX = (rand() % (bt.getWidth() * 2 + 1)) - bt.getWidth();
        offsetY = (rand() % (bt.getLength() * 2 + 1)) - bt.getLength();

        // Skip if no movement
        if (offsetX == 0 && offsetY == 0)
            continue;

        targetX = getX() + offsetX;
        targetY = getY() + offsetY;
        if (bt.isValid(targetX, targetY))
        {
            fire(offsetX, offsetY, bt, outfile);
            if (getNumOfRobotTerminated() >= 3)
                setUpgradePermission(true);
            break;
        }
    }
}

// UltimateRobot class
UltimateRobot::UltimateRobot(string t, string n, int x, int y, char s) : TerminatorRoboCop(t, n, x, y, s), RoboTank(t, n, x, y, s), RoboCop(t, n, x, y, s), Terminator(t, n, x, y, s), Madbot(t, n, x, y, s), BlueThunder(t, n, x, y, s), MovingRobot(t, n, x, y, s), SeeingRobot(t, n, x, y, s), SteppingRobot(t, n, x, y, s), Robot(t, n, x, y, s)
{
}

void UltimateRobot::takeTurn(Battlefield &bt, ofstream &outfile)
{
    Terminator::takeTurn(bt, outfile);

    for (int i = 0; i < 3; i++)
    {
        RoboTank::takeTurn(bt, outfile);
    }
}

// BomberMan class
// BomberMan::BomberMan(string t, string n, int x, int y, char s) : Robot(t, n, x, y, s), MovingRobot(t, n, x, y, s), ShootingRobot(t, n, x, y, s)
// {
// }

// War class
War::War(const string &input)
{
    robotPlaying;
    robotWaiting;
    ifstream infile(input);

    if (infile.fail())
    {
        cout << "Error opening file: " << input << endl;
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
            if (robotPlaying.getListLength() == 0)
                break;
            for (int i = 0; i < robotPlaying.getListLength(); i++)
            {
                if (robotPlaying.getNodeAt(i)->getRobot()->getX() == tempX && robotPlaying.getNodeAt(i)->getRobot()->getY() == tempY)
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
    noOfRobotWaiting = 0;

    // Initialize other data member
    battlefield = Battlefield(w, l, robotPlaying);

    totalSteps = steps;
    currentStep = 0;
    totalRobots = robotsRemaining = noOfRobotPlaying;
    robotsDied = 0;
}

void War::changePosition(Robot *&r)
{
    int tempX, tempY;
    bool valid_position = false;
    while (!valid_position)
    {
        valid_position = true;
        tempX = rand() % battlefield.getWidth();
        tempY = rand() % battlefield.getLength();
        for (int i = 0; i < robotPlaying.getListLength(); i++)
        {
            if (robotPlaying.getNodeAt(i)->getRobot()->getX() == tempX && robotPlaying.getNodeAt(i)->getRobot()->getY() == tempY)
            {
                valid_position = false;
                break;
            }
        }
    }
    r->setX(tempX);
    r->setY(tempY);
}

void War::initializeRobot(string tt, string tn, int tx, int ty)
{
    Robot *newRobot = nullptr;

    if (tt == "RoboCop")
    {
        newRobot = new RoboCop(tt, tn, tx, ty, 'R');
    }
    else if (tt == "Terminator")
    {
        newRobot = new Terminator(tt, tn, tx, ty, 'T');
    }
    else if (tt == "TerminatorRoboCop")
    {
        newRobot = new TerminatorRoboCop(tt, tn, tx, ty, 'N');
    }
    else if (tt == "BlueThunder")
    {
        newRobot = new BlueThunder(tt, tn, tx, ty, 'B');
    }
    else if (tt == "Madbot")
    {
        newRobot = new Madbot(tt, tn, tx, ty, 'M');
    }
    else if (tt == "RoboTank")
    {
        newRobot = new RoboTank(tt, tn, tx, ty, 'O');
    }
    else if (tt == "UltimateRobot")
    {
        newRobot = new UltimateRobot(tt, tn, tx, ty, 'U');
    }
    else
    {
        cout << "Invalid Robot Type \"" << tt << "\"\n";
        exit(EXIT_FAILURE);
    }

    robotPlaying.appendRobot(newRobot);

    newRobot = nullptr;
}

bool War::isPlayingEmpty() const
{
    return robotPlaying.getListLength() == 0;
}

bool War::isWaitingEmpty() const
{
    return robotWaiting.getQueueLength() == 0;
}

void War::terminateRobot(Robot &r)
{
    robotPlaying.removeRobot(r); // remove from linked list
    battlefield.removeRobot(r);  // remove from battlefield

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
        robotWaiting.enqueue(&r);
        noOfRobotWaiting++;
    }
}

void War::promoteRobot(Robot &r)
{
    Robot *promotedRobot = nullptr;
    if (r.getType() == "RoboCop" || r.getType() == "Terminator")
    {
        cout << r.getType() << " " << r.getName() << " has been upgraded to TerminatorRoboCop!" << endl;
        promotedRobot = new TerminatorRoboCop("TerminatorRoboCop", r.getName(), r.getX(), r.getY(), 'N');
    }
    else if (r.getType() == "BlueThunder")
    {
        cout << r.getType() << " " << r.getName() << " has been upgraded to MadBot!" << endl;
        promotedRobot = new Madbot("Madbot", r.getName(), r.getX(), r.getY(), 'M');
    }
    else if (r.getType() == "Madbot")
    {
        cout << r.getType() << " " << r.getName() << " has been upgraded to RoboTank!" << endl;
        promotedRobot = new RoboTank("RoboTank", r.getName(), r.getX(), r.getY(), 'O');
    }
    else if (r.getType() == "TerminatorRoboCop" || r.getType() == "RoboTank")
    {
        cout << r.getType() << " " << r.getName() << " has been upgraded to UltimateRobot!" << endl;
        promotedRobot = new UltimateRobot("UltimateRobot", r.getName(), r.getX(), r.getY(), 'U');
    }
    robotPlaying.replaceRobot(&r, promotedRobot);
    battlefield.removeRobot(r);
    battlefield.updatePosition(promotedRobot, promotedRobot->getX(), promotedRobot->getY());
    delete &r;
}

void War::startWar(const string &output)
{
    ofstream outfile(output);
    if (outfile.fail())
    {
        cout << "Error opening file: " << output << endl;
        exit(EXIT_FAILURE);
    }

    while (currentStep < totalSteps && robotsRemaining > 1)
    {
        currentStep++;
        cout << "Current step: " << currentStep << endl
             << endl;
        outfile << "Current step: " << currentStep << endl
                << endl;
        battlefield.displayField();
        battlefield.displayField(outfile);
        cout << "Action:\n";
        outfile << "Action:\n";
        for (int i = 0; i < robotPlaying.getListLength(); i++)
        {
            Robot *currentRobot = robotPlaying.getNodeAt(i)->getRobot();

            int prevX = currentRobot->getX();
            int prevY = currentRobot->getY();

            currentRobot->takeTurn(battlefield, outfile);
            cout << endl;

            battlefield.removeRobot(prevX, prevY);
            battlefield.updatePosition(currentRobot, currentRobot->getX(), currentRobot->getY());

            for (int i = 0; i < 3; i++)
            {
                if (&currentRobot->getRobotTerminated(i) == nullptr)
                    break;
                else
                {
                    terminateRobot(currentRobot->getRobotTerminated(i));
                    battlefield.updatePosition(currentRobot, currentRobot->getX(), currentRobot->getY());
                }
            }
            currentRobot->resetRobotTerminated();
            if (currentRobot->getUpgradePermission() && currentRobot->getType() != "UltimateRobot")
                promoteRobot(*currentRobot);
        }
        if (!isWaitingEmpty() && robotsRemaining > 0)
        {
            Robot *returnRobot;
            robotWaiting.dequeue(returnRobot);
            noOfRobotWaiting--;

            changePosition(returnRobot);
            battlefield.updatePosition(returnRobot, returnRobot->getX(), returnRobot->getY());
            robotPlaying.appendRobot(returnRobot);
            noOfRobotPlaying++;
            cout << returnRobot->getType() << " " << returnRobot->getName() << " has respawned from the queue at position ("
                 << returnRobot->getX() << ", " << returnRobot->getY() << ").\n";
            outfile << returnRobot->getType() << " " << returnRobot->getName() << " has respawned from the queue at position ("
                    << returnRobot->getX() << ", " << returnRobot->getY() << ").\n";
        }
        cout << "\nLive Statistics:\n";
        cout << "-----------------------------\n";
        cout << "\nNumber of robots terminated by...\n";

        outfile << "\nLive Statistics:\n";
        outfile << "-----------------------------\n";
        outfile << "\nNumber of robots terminated by...\n";
        for (int i = 0; i < robotPlaying.getListLength(); i++)
        {
            Robot *currentRobot = robotPlaying.getNodeAt(i)->getRobot();
            cout << currentRobot->getType() << " " << currentRobot->getName() << ": ";
            outfile << currentRobot->getType() << " " << currentRobot->getName() << ": ";
            cout << currentRobot->getNumOfRobotTerminated() << endl;
            outfile << currentRobot->getNumOfRobotTerminated() << endl;

        }

        cout << "\n-----------------------------\n";
        outfile << "\n-----------------------------\n";
        cout << "\nRemaining lives:\n";
        outfile << "\nRemaining lives of:\n";
        for (int i = 0; i < robotPlaying.getListLength(); i++)
        {
            Robot *currentRobot = robotPlaying.getNodeAt(i)->getRobot();
            cout << currentRobot->getType() << " " << currentRobot->getName() << ": ";
            outfile << currentRobot->getType() << " " << currentRobot->getName() << ": ";
            cout << currentRobot->getRemainingLives() << endl;
            outfile << currentRobot->getRemainingLives() << endl;
        }
        cout << "\n-----------------------------\n";
        outfile << "\n-----------------------------\n";
        cout << "\n\n";
        outfile << "\n\n";
    }
    if (robotsRemaining <= 0)
    {
        cout << "All robots have been terminated!\n Game Over!!!\n";
        outfile << "All robots have been terminated!\n Game Over!!!\n";
    }
    else if (currentStep >= totalSteps && robotsRemaining > 1)
    {
        cout << "The war has reached the maximum number of steps.\nDraw!!!\n";
        outfile << "The war has reached the maximum number of steps.\nDraw!!!\n";
    }
    else
    {
        cout << "War end.\nThe winner is " << robotPlaying.getNodeAt(0)->getRobot()->getName() << "!!!\nCongratulations!!!\n";
        outfile << "War end.\nThe winner is " << robotPlaying.getNodeAt(0)->getRobot()->getName() << "!!!\nCongratulations!!!\n";
    }
    outfile.close();
}
