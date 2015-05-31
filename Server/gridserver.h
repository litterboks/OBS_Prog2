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
private:
	int cellValue(int x, int y) const;
	int sendMsg(int text, int msgid, long type);
	int regVehicle(char name, int pid); 
	void removeVehicle(char name);
	int placeVehicle(char name, int pid);
	std::string gridToString(std::vector<char> grid);
	FILE* pipe;
	int width;
	int height;
	std::map<char, int> registeredVehicles;
	std::vector<char> grid;
};
