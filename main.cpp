#include <iostream>
#include <ctime>
#include <cstdlib>
#include "robot.h"
using namespace std;

int main()
{
    srand(time(0));
    War newGame("input.txt");
    newGame.startWar();
    return 0;
}
