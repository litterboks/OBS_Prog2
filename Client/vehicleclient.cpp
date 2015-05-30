#include "vehicleclient.h"

using namespace std;

int sending(char text, int msgid, int type);
void signal_handler(int sig)
{
    if(sig == SIGTERM)
    {
    cout << "Vehicle has been eliminated." << endl;
    exit(EXIT_SUCCESS);
    }
}


int main(int argc, char* argv[])
{
        if(argc < 2) //Wenn kein Wagenname eingegeben wurde
    {
	    cout << "Usage!!" << endl;
        return -1;
    }

    (void) signal(SIGTERM, signal_handler);
    char name = argv[1][0];
    char moving;
    int msgid = -1;	/* Message Queue ID */
//    int msgid2 = -1;
    //int vtype = 0;
    //int mtype = 1;
    int ret;
    int i;

    if (name <'A' || name > 'Z') //Wenn der Wagenname kein Großbuchstabe war
    {
	cout << "Buchstabe zwischen A und Z bitte!" << endl;
        return -1;
    }


    if((msgid = msgget(KEY,PERM))==-1 ) //Message Queue erstellen
    {
	cout << "Message Queue not found!" << endl;
        return -1;
      //return EXIT_FAILURE;
    }

    /*if((msgid2 = msgget(KEY2, PERM)) == -1)
    {
        return -1;
    }*/
//	cout << "MSGID: " << msgid << endl;
    i = sending(name , msgid, 'R');

    if(i == -1)
    {
        cout << "Registration FAILED." << endl;
        return -1;
    }
    else
    {
        char position[3];

        //Nachricht lesen
        int msgrcv(int msgid2, void *position, size_t msgsize, long msgtype, int flags);

        cout << "Registration OK. Start position: " << position << "." << endl;
    }

   while(1==1)
    {

        cout << "Enter Move: ";
        cin >> moving;

        switch(moving)
        {
            case 'N' : 
            case 'O' :
            case 'S' :
            case 'W' :
            case 'T' : ret = sending(name ,msgid, moving); break;
            default: cout << "Ungültiger Move" << endl;
        }

        if(ret == -1)
        {
            cout << "Error while movement" << endl;
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
   if (msgsnd(msgid, &msg, sizeof(message_t)-sizeof(int) , 0) == -1) //Nachricht in Message Queue schreiben
   {
         //return EXIT_FAILURE;
	 cout << errno <<endl;
         return -1;
   }
    return 0;

}
