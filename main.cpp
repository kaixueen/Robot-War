#include <iostream>
using namespace std;

int main()
{
    int l = 50;
    int w = 40;
    char **cellArr = new char *[l];

    for (int i = 0; i < l; i++)
    {
        cellArr[i] = new char[w];
        for (int j = 0; j < w; j++)
        {
            cellArr[i][j] = ' ';
        }
    }
    for (int i = 0; i < l + 2; i++)
    {
        for (int j = 0; j < w + 2; j++)
        {
            if (i == 0 || i == l + 1 || j == 0 || j == w + 1)
                cout << '*';
            else
                cout << cellArr[i - 1][j - 1];
        }
        cout << endl;
    }
}
