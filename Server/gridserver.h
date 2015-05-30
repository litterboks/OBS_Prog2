#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include <vector>
#include <string>

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
