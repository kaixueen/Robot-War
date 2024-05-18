#include <iostream>
#include "robot.h"
using namespace std;

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