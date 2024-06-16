/**********|**********|**********|
Program: robot.cpp / robot.h
Course: CCP6124-OOPDS
Trimester: 2410
Group members:
1.  Name: NG KAI XUEN
    ID  : 1211111363
2.  Name: SIAO WEI CHENG
    ID  : 1211108953
3.  Name: TAN YEW WEN
    ID  : 1211109701
Lecture Section: TC4L
Tutorial Section: T14L
Email: 1211111363@student.mmu.edu.my
Phone: 011-10669638
**********|**********|**********/

#include <iostream>
#include <ctime>
#include <cstdlib>
#include "robot.h"
using namespace std;

int main()
{
    srand(time(0));
    War newGame("test.txt");
    cout << "Yo! Welcome to Robot War!!!\n\n";
    cout << "##### User Guide #####\n";
    cout << "\'R\' represents RoboCop, \'T\' represents Terminator, \'N\' represents TerminatorRoboCop,\n";
    cout << "\'B\' represents BlueThunder, \'M\' represents Madbot, \'O\' represents RoboTank,\n";
    cout << "\'U\' represents UltimateRobot.\n\n";
    cout << "Now... War Start!\n\n";

    newGame.startWar("output.txt");
    return 0;
}
