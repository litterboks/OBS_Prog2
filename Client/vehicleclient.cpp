#include "vehicleclient.h"
using namespace std;

int msgid_r = -1;	/* Message Queue ID */
int msgid_w = -1;
char name;

int sending(char text, int msgid, int type);
void signal_handler(int sig)
{
	switch (sig) {
	case SIGTERM:

		std::cout << std::endl << "Vehicle has been eliminated." << std::endl;
		exit(EXIT_SUCCESS);
		break;
	case SIGINT:
		sending(name, msgid_w, 'T');
		std::cout << "Vehicle has been eliminated." << std::endl;
		exit(EXIT_SUCCESS);
		break;

	}
}

int main(int argc, char* argv[])
{
	if (argc < 2) { //Wenn kein Wagenname eingegeben wurde
		cout << "Usage: vehicleclient [A-Z]" << endl;
		return -1;
	}

	(void) signal(SIGTERM, signal_handler);
	(void) signal(SIGINT, signal_handler);
	message_t msg;
	name = argv[1][0];
	char moving;

	//Wenn der Wagenname kein Großbuchstabe war
	if (name < 'A' || name > 'Z') {
		cout << "Buchstabe zwischen A und Z bitte!" << endl;
		return -1;
	}

	//Message Queues finden
	if ((msgid_r = msgget(KEY2, PERM)) == -1) {
		cout << "Read Message Queue not found!" << endl;
		return -1;
	}
	if ((msgid_w = msgget(KEY, PERM)) == -1) {
		cout << "Write Message Queue not found!" << endl;
		return -1;
	}

	if (sending(name, msgid_w, 'R')) {
		cout << "Registration FAILED." << endl;
		return -1;
	}

	if (msgrcv(msgid_r,  &msg, sizeof(message_t) - sizeof(long), 0 , 0) == -1) {
		std::cout << "Cant' receive message" << std::endl;
	}

	if (msg.mType == 'R') {
		//	cout << "Message: "<< msg.mText << endl;
		int x = msg.mText / 1000;
		int y = msg.mText % 1000;
		cout << "Registration OK. Start position: " << x << "." << y << endl;
	} else if (msg.mType == 'E') {
		cout << "Car does already exist" << endl;
		return -1;
	} else {
		cout << "Unknown error at registration" << endl;
	}


	while (1 == 1) {
		cout << "Enter Move: ";
		moving = cin.get();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		switch (moving) {
		case 'N' :
		case 'E' :
		case 'S' :
		case 'W' :
		case 'T' :
			if ((sending(name , msgid_w, moving)) == -1) {
				return -1;
			}
			break;
		default:
			cout << "Ungültiger Move" << endl;
		}
	}

	return 0;
}

int sending(char text, int msgid, int type)
{

	//Nachricht verschicken
	message_t msg;	/* Buffer fuer Message */
	msg.mText = text;
	msg.mType = type;
	msg.mPID = getpid();
	if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long) , 0) == -1) { //Nachricht in Message Queue schreiben
		//return EXIT_FAILURE;
		cout << errno << endl;
		return -1;
	}
	return 0;

}
