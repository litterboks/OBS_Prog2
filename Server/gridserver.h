#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <string>

#define MAX_DATA 255
#define KEY 14049
#define KEY2 14062
#define PERM 0660

typedef struct {
	long mType;
	int mText;
} message_t;

class Vehicle
{

};
class Server
{
public:
	Server(int width, int height);
	void run_server();
	void send_display(std::string message);
private:
	int sendMsg(char text, int msgid, int type);
	int regVehicle(char name); 
	FILE* pipe;
	int width;
	int height;
	std::vector<char> grid;
	std::map<char, Vehicle*> vehicles;
};
