#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

int main()
{
    ifstream infile("input.txt");
    if (infile.fail())
    {
        cout << "Error opening file: " << endl;
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
            ss >> l;
        temp = "";
    }
    cout << w << " " << l;
    return 0;
}