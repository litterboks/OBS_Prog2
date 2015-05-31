#include <string>
#include "gridserver.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <signal.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdlib.h>

void print_usage();
void run_server();

int msgid = -1;
int msgid2 = -1;

void signal_handler(int sig)
{
	switch (sig) {
	case SIGHUP:
		break;
	case SIGINT:
		msgctl(msgid, IPC_RMID, NULL);
		msgctl(msgid2, IPC_RMID, NULL);
		remove("../pipe");
		exit(EXIT_SUCCESS);
	case SIGQUIT:
		break;
	}
}

Server::Server(int width, int height)
{
	this->width = width;
	this->height = height;
	grid.resize(width * height);
	std::fill(grid.begin(), grid.end(), ' ');
	int mkfifo(const char *path, mode_t mode);
	if (mkfifo("../pipe", 0660) == -1) {
		return;
	}
}

int Server::cellValue(int x, int y) const
{
	return y * width + x;
}

int main(int argc, char* argv[])
{
	int c = 0;
	int countX = 0;
	int countY = 0;
	int width;
	int height;
	(void) signal(SIGHUP, signal_handler);
	(void) signal(SIGINT, signal_handler);
	(void) signal(SIGQUIT, signal_handler);

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

int Server::sendMsg(int text, int msgid, long type)
{
	//Nachricht verschicken
	message_t msg;  /* Buffer fuer Message */
	msg.mText = text;
	msg.mType = type;
	if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long) , 0) == -1) { //Nachricht in Message Queue schreiben
		//return EXIT_FAILURE;
		std::cout << errno << std::endl;
		return -1;
	}
	return 0;

}

int Server::placeVehicle(char name, int pid)
{
	if (registeredVehicles.count(name) == 0) {
		registeredVehicles[name] = pid;

		for (unsigned int i = 0; i < grid.size(); i++) {
			if (grid[i] == ' ') {
				grid[i] = name;
				return 0;
			}
		}
	}
	return -1;
}

int Server::regVehicle(char name, int pid)
{
	int ret = 0;
	if ((ret = placeVehicle(name, pid) == 0)) {
		std::cout << "Registered Vehicle: " << name << "  PID: " << registeredVehicles[name] << std::endl;
		sendMsg(ret, msgid2, 'R');
		return 0;
	} else {
		sendMsg(ret, msgid2, 'E');
		return -1;
	}
}

void Server::removeVehicle(char name)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (grid[cellValue(x, y)] == name) {
				grid[cellValue(x, y)] = ' ';
			}
		}
	}
	return;
}
std::string Server::gridToString(std::vector<char> grid)
{
	std::string s;
	//Erste Reihe mit # füllen 1 mehr als Feld Breit
	for (int i = 0; i <= width + 1; i++) {
		s.push_back('#');
	}
	s.push_back('\n');

	for (int y = 0; y < height; y++) {
		s.push_back('#');
		for (int x = 0; x < width; x++) {
			s.push_back(grid[cellValue(x, y)]);
		}
		s.push_back('#');
		s.push_back('\n');
	}

	//Letzte Reihe mit # füllen
	for (int i = 0; i <= width + 1; i++) {
		s.push_back('#');
	}
	s.push_back('\n');

	return s;
}

void Server::run_server()
{
	message_t msg;	/* Buffer fuer Message */

	/* Message Queue neu anlegen */
	if ((msgid = msgget(KEY, PERM | IPC_CREAT | IPC_EXCL)) == -1) {
		/* error handling */
		std::cout << "Error creating message queue" << std::endl;
		std::cout << "Errno: " << errno << " msgid: " << msgid << std::endl;
		exit(EXIT_FAILURE);
	}

	if ((msgid2 = msgget(KEY2, PERM | IPC_CREAT | IPC_EXCL)) == -1) {
		/* error handling */
		std::cout << "Error creating message queue" << std::endl;
		std::cout << "Errno: " << errno << " msgid: " << msgid << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "RUN SERVER" << std::endl;
	while (1 == 1) {
		if (msgrcv(msgid,  &msg, sizeof(message_t) - sizeof(long), 0 , 0) == -1) {
			/* error handling */
			std::cout << "Cant' receive message" << std::endl;
		}

		switch (msg.mType) {
		case 'R':
			regVehicle(msg.mText, msg.mPID);
			send_display(gridToString(grid));
			break;

		case 'N':
		case 'W':
		case 'S':
		case 'E':
			send_display(gridToString(grid));
			std::cout << "Move direction " << (char) msg.mType << std::endl;
			break;
		case 'T':
			removeVehicle(msg.mText);
			kill(msg.mPID, SIGTERM);
			registeredVehicles.erase(msg.mText);
			send_display(gridToString(grid));
			break;
		default:
			break;
		}
	}

	return;
}


