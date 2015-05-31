#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <set>
#include <string>

#define MAX_DATA 255
#define KEY 14049
#define KEY2 14062
#define PERM 0660

typedef struct {
	long mType;
	int mText;
	int mPID;
} message_t;

class Server
{
public:
	Server(int width, int height);
	void run_server();
	void send_display(std::string message);
	int getPID(char name);
private:
	int width;
	int height;

	int cellValue(int x, int y) const;
	int sendMsg(int text, int msgid, long type);

	int moveVehicle(char name, char direction);
	int step(int x, int y, char direction);

	std::map<char, int> registeredVehicles;
	int regVehicle(char name, int pid); 
	int placeVehicle(char name, int pid);
	void removeVehicle(char name);

	FILE* pipe;
	std::vector<char> grid;
	std::string gridToString(std::vector<char> grid);
};
