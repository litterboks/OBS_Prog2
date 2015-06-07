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

int read_msgid = -1;
int write_msgid = -1;

void signal_handler(int sig)
{
	switch (sig) {
	case SIGHUP:
		break;
	case SIGINT:
		msgctl(read_msgid, IPC_RMID, NULL);
		msgctl(write_msgid, IPC_RMID, NULL);
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
			//std::cout << "Case ?" << std::endl;
			print_usage();
			break;
		default:
			//std::cout << "Default Case" << std::endl;
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


int Server::sendMsg(int text, int msgid, long type)
{
	//Nachricht verschicken
	message_t msg; //Buffer für Message
	msg.mText = text;
	msg.mType = type;
	if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long) , 0) == -1) {
		return -1;
	}
	return 0;

}

int Server::placeVehicle(char name, int pid)
{
	if (registeredVehicles.count(name) == 0) {
		registeredVehicles[name] = pid;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {

				if (grid[cellValue(x, y)] == ' ') {
					grid[cellValue(x, y)] = name;
					int encoded = (1000 * x + y);
					return encoded;
				}
			}
		}
	}
	return -1;
}

int Server::regVehicle(char name, int pid)
{
	int ret = placeVehicle(name, pid);
	if (ret != -1) {
		sendMsg(ret, w_msgid, 'R');
		return 0;
	} else {
		sendMsg(ret, w_msgid, 'E');
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
	kill(registeredVehicles[name], SIGTERM);
	registeredVehicles.erase(name);
	return;
}

typedef struct {
	int x;
	int y;
} coordinates;

int Server::step(int x, int y, char direction)
{
	char name = grid[cellValue(x, y)];
	std::map<char, coordinates> directions;
	directions['N'].y = -1;
	directions['N'].x = 0;
	directions['S'].y = 1;
	directions['S'].x = 0;
	directions['W'].y = 0;
	directions['W'].x = -1;
	directions['E'].y = 0;
	directions['E'].x = 1;

	if (x + directions[direction].x < 0 || y + directions[direction].y < 0 || x + directions[direction].x >= width || y + directions[direction].y >= height) {
		removeVehicle(name);
		return 0;
	}
	grid[cellValue(x, y)] = ' ';

	char tmp_field;
	int destinationX = x + directions[direction].x;
	int destinationY = y + directions[direction].y;

	if ((tmp_field = grid[cellValue(destinationX, destinationY)]) == ' ') {
		grid[cellValue(destinationX, destinationY)] = name;
	} else {
		removeVehicle(name);
		removeVehicle(tmp_field);
		return 0;
	}
	return 0;
}

int Server::moveVehicle(char name, char direction)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (grid[cellValue(x, y)] == name) {

				step(x, y, direction);
				return 0;
			}
		}
	}
	return 0;
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

void Server::initQueues()
{
	//Möglicherweise noch geöffnete queues schließen
	msgctl(msgget(KEY, PERM), IPC_RMID, NULL);
	msgctl(msgget(KEY2, PERM), IPC_RMID, NULL);

	//Message Queues anlegen
	if ((r_msgid = msgget(KEY, PERM | IPC_CREAT | IPC_EXCL)) == -1) {
		/* error handling */
		std::cout << "Error creating message queue" << std::endl;
		std::cout << "Errno: " << errno << " msgid: " << r_msgid << std::endl;
		exit(EXIT_FAILURE);
	}

	if ((w_msgid = msgget(KEY2, PERM | IPC_CREAT | IPC_EXCL)) == -1) {
		/* error handling */
		std::cout << "Error creating message queue" << std::endl;
		std::cout << "Errno: " << errno << " msgid: " << w_msgid << std::endl;
		exit(EXIT_FAILURE);
	}
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

	//Buffer fuer Message
	message_t msg;
	//Queues erstellen
	initQueues();
	write_msgid = w_msgid;
	read_msgid = r_msgid;

//	std::cout << "RUN SERVER" << std::endl;
	while (1 == 1) {
		if (msgrcv(r_msgid,  &msg, sizeof(message_t) - sizeof(long), 0 , 0) == -1) {
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
			moveVehicle(msg.mText, msg.mType);
			send_display(gridToString(grid));
			break;
		case 'T':
			removeVehicle(msg.mText);
			send_display(gridToString(grid));
			break;
		default:
			break;
		}
	}

	return;
}
