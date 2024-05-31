#include <iostream>
using namespace std;
class Robot
{
private:
    string robotName;
    string robotType;
    int robotPositionX, robotPositionY;
    int remainingLives;
    Robot *robotTerminated[3];
    int numOfRobotTerminated;
    bool upgradePermission;

public:
    Robot();                                 // Default constructor
    Robot(string t, string n, int x, int y); // Parameterized constructor
    Robot(const Robot &r);                   // Copy constructor
    Robot &operator=(const Robot &right);    // Assignment operator overloading
    virtual ~Robot();                        // Destructor

    string getName() const; // Return robot name
    string getType() const; // Return robot type

    int getX() const; // Return robot x coordinate
    int getY() const; // Return robot y coordinate
    void setX(int x); // Set robot x coordinate
    void setY(int y); // Set robot y coordinate

    bool stillGotLive() const;     // Remaining lives > 0?
    void setRemainingLives(int l); // Set remaining lives
    int getRemainingLives() const; // Return remaining lives

    void setRobotTerminated(Robot &r);      // store robot terminated
    Robot &getRobotTerminated(int n) const; // return robot terminated
    int getNumOfRobotTerminated() const;    // return number of robot terminated
    void resetRobotTerminated();            // reset robot terminated to nullptr

    // virtual void takeTurn(Battlefield &bt) = 0; // Pure virtual function
    bool getUpgradePermission() const;          // Valid for upgrade?
    void setUpgradePermission(bool p);          // Set upgradePermission
};

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
    for (int i = 0; i < 3; i++)
        delete robotTerminated[i];
}

struct RobotNode // struct for making linked list & queue
{
    Robot *rb;
    RobotNode *nextRobot;
};

// Battlefield class
class Battlefield
{
private:
    int width, length;
    char emptyCell, robotCell, boundary;
    Robot ***cellArr;

public:
    Battlefield(int w, int l, RobotNode *rn); // initialize field using linked list

    void displayField() const;
    bool isEmpty(int x, int y) const;
    bool isValid(int x, int y) const;

    ~Battlefield();
};

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

bool Battlefield::isEmpty(int x, int y) const
{
    return cellArr[y][x] == nullptr;
}

bool Battlefield::isValid(int x, int y) const
{
    return x >= 0 && x < width && y >= 0 && y < length;
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

Battlefield::~Battlefield()
{
    for (int i = 0; i < length; i++)
    {
        delete[] cellArr[i];
    }
    delete[] cellArr;
}

int main()
{
    int totalSteps;
    int currentStep;
    int totalRobots;
    int noOfRobotPlaying;
    int robotsRemaining;
    int robotsDied;

    RobotNode *headRobot; // linked list of robot playing

    RobotNode *frontWaiting; // queue of robot waiting
    RobotNode *rearWaiting;
    int noOfRobotWaiting;

    // Initialize waiting queue
    frontWaiting = rearWaiting = nullptr;
    noOfRobotWaiting = 0;

    // Initialize other data member
    totalSteps = 10;
    currentStep = 0;
    robotsDied = 0;

    // Initialize the linked list of robots
    headRobot = new RobotNode();
    headRobot->rb = new Robot("Robot", "r1", 1, 2);
    headRobot->nextRobot = new RobotNode();
    headRobot->nextRobot->rb = new Robot("Robot", "r2", 3, 2);
    headRobot->nextRobot->nextRobot = new RobotNode();
    headRobot->nextRobot->nextRobot->rb = new Robot("Robot", "r3", 3, 8);
    headRobot->nextRobot->nextRobot->nextRobot = new RobotNode();
    headRobot->nextRobot->nextRobot->nextRobot->rb = new Robot("Robot", "r4", 6, 2);
    headRobot->nextRobot->nextRobot->nextRobot->nextRobot = nullptr;

    Battlefield battlefield(20, 20, headRobot);
    totalRobots = robotsRemaining = noOfRobotPlaying = 4;
    battlefield.displayField();

    // Clean up dynamically allocated memory
    RobotNode* current = headRobot;
    while (current != nullptr) {
        RobotNode* next = current->nextRobot;
        delete current->rb;
        delete current;
        current = next;
    }
}

