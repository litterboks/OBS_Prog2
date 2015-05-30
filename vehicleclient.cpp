#include <iostream>
#include <getopt.h>
#include <unistd.h>
//#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        return -1;
    }


    char name = argv[1][0];
    char moving;

    cout << "Registration OK" << endl;
    cout << name << endl;
    cout << "Enter Move: ";
    cin >> moving;
    cout << "The input was" << moving << endl;
    return 0;
}
