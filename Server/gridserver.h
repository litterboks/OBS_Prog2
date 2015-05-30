#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include <vector>

class Server
{
public:
Server(int width, int height);
void run_server();
private:
int width;
int height;
std::vector<char> grid;
};
