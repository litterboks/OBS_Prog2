#include <iostream>
#include <getopt.h>
#include <unistd.h>
//#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[])
{
        if(argc < 2) //Wenn kein Wagenname eingegeben wurde
    {
        return -1;
    }

    char name = argv[1][0];
    char moving;
    int w = 0;
    int h = 0;



    if (name <'A' || name > 'Z') //Wenn der Wagenname kein Großbuchstabe war
    {
        return -1;
    }

    //Wagennamen schicken und registration überprüfen

    if(/*keine Startposition erhalten */)
    {
        cout << "Registration FAILED." << endl;
        return -1;
    }
    else
    {
    cout << "Registration OK. Start position: " << w << "." << h << endl;

    while(/*Auto nicht geschrottet/ Signal != SIGTERM */)
    {
        cout << "Enter Move: ";
        cin >> moving;

        switch(moving)
        {
            case 'N' : //Queue schicken mit dem Move
            case 'O' :
            case 'S' :
            case 'W' :
            case 'T' :     //cout << "The input was " << moving << endl;
                    break;
            default: cout << "Ungültiger Move" << endl;
                    break;
        }

        // ?? Überprüfung ob Auto geschrottet oder nicht
    }

    cout << "Vehicle has been eliminated." << endl;
    }

    return 0;
}
