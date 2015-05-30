#include <iostream>
#include <unistd.h>
#include <signal.h>
//#include <stdlib.h>
#include <griddisplay.h>

void signal_handler(int sig)
{
    if(sig == SIGINT)
    {
        exit(EXIT_SUCCESS);
    }
}


using namespace std;

void Grid::Griddisplay ()


int main()
{
    int height = 0;
    int width = 0;
(void) signal(SIGINT, signal_handler);

    while (1==1)
    {
       int x =  Grid::Griddisplay();

        if(x == -1)
            return -1;
    }



    return 0;
}

int Grid::Griddisplay ()
{
   ifstream f;
   f.open("../pipe");

    if(f.is_open() !=true)
    {
        cout << "Error with Signal" << endl;
        return -1;
    }

    cout << f.rdbuff() << endl;
    return 0;
}
