#include "gridserver.h"
#include <string>

void print_usage();
void run_server();
Server::Server(int width, int height)
{
	this->width = width;
	this->height = height;
	grid.resize(width * height);
}

int main(int argc, char* argv[])
{
	int c = 0;
	int countX = 0;
	int countY = 0;
	int width;
	int height;

	while ((c = getopt(argc, argv, "x:y:")) != EOF) {
		switch (c) {
		case 'x':
			width = strtol(optarg, NULL, 0);
			if (width != 0) {
				countX++;
			}
			break;
		case'y':
			height = strtol(optarg, NULL, 0);
			if (height != 0) {
				countY++;
			}
			break;
		case'?':
			std::cout << "Case ?" << std::endl;
			print_usage();
			break;
		default:
			std::cout << "Default Case" << std::endl;
			print_usage();
			return -1;
		}
	}
	if (countX != 0 && countY != 0) {
		Server s(width, height);
		s.run_server();
	} else {
		print_usage();
	}
	return 0;
}

void print_usage()
{
	std::cout << "Usage: ./gridserver [-x width] [-y height]\nHeight and width have to be greather than 0" << std::endl;
	return;
}

void Server::run_server()
{
	std::cout << "RUN SERVER" << std::endl;
	std::cout << "Width: " << width << std::endl << "Height: " << height << std::endl;
	while(1==1){
		
	};
	          return;
          }

