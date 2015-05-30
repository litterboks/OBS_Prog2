#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <fstream>
#include <sys/msg.h>
#include <stdlib.h>
#include "griddisplay.h"

void signal_handler(int sig)
{
	if (sig == SIGINT) {
		exit(EXIT_SUCCESS);
	}
}


using namespace std;

int Display::Griddisplay()
{
	std::ifstream f;
	f.open("../pipe");

	if (f.is_open() != true) {
		cout << "Error with Signal" << endl;
		return -1 ;
	}
	cout << f << endl;
	return 0;
}


int main()
{
	Display d;
	(void) signal(SIGINT, signal_handler);

	while (1 == 1) {
		int x = d.Griddisplay();
		if (x == -1) {
			return -1;
		}
	}
	return 0;
}
