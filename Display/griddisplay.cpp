#include "griddisplay.h"

void signal_handler(int sig)
{
	if (sig == SIGINT) {
		exit(EXIT_SUCCESS);
	}
}


using namespace std;

int Display::printGrid()
{
	std::ifstream f;
	f.open("../pipe");
	if (f.is_open()) {

		char c = f.get();
		while (f.good()) {
			std::cout << c;
			c = f.get();
		}
	}
	return 0;
}


int main()
{
	Display display;
	(void) signal(SIGINT, signal_handler);

	while (1 == 1) {
		int x = display.printGrid();
		if (x == -1) {
			return -1;
		}
	}
	return 0;
}
