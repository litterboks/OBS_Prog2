#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include <vector>
#include <string>

#define MAX_DATA 255
#define KEY 14049
#define KEY2 14062
#define PERM 0660

typedef struct {
	long mType;
	char mText[MAX_DATA];
} message_t;


class Server
{
public:
	Server(int width, int height);
	void run_server();
	void send_display(std::string message);
private:
	FILE* pipe;
	int width;
	int height;
	std::vector<char> grid;
};
