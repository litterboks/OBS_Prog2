#include <string>
#include "gridserver.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <signal.h>
#include <sys/msg.h>

void print_usage();
void run_server();
Server::Server(int width, int height)
{
	this->width = width;
	this->height = height;
	grid.resize(width * height);

	int mkfifo(const char *path, mode_t mode);
	if (mkfifo("../pipe", 0660) == -1) {
		return;
	}
}

void signal_handler(int sig)
{
switch (sig){
	case SIGHUP:
		break;
	case SIGINT:
		exit(EXIT_SUCCESS);
	case SIGQUIT:
		break;
}
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

void Server::send_display(std::string message)
{
	std::ofstream out_pipe;
	out_pipe.open("../pipe");
	out_pipe << message;
}

void Server::run_server()
{
	int msgid;
	std::cout << "RUN SERVER" << std::endl;
	if( ( msgid = msgget(12345,0666|IPC_CREAT|IPC_EXCL ))==-1 ){

	}
	(void) signal(SIGHUP, signal_handler);
	(void) signal(SIGINT, signal_handler);
	(void) signal(SIGQUIT, signal_handler);
	while (1==1){
	
	}

	remove("../pipe");
	return;
}
