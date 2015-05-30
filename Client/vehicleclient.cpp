#include "vehicleclient.h"

using namespace std;

int sending(char x, int msgid, int type);
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
        return -1;
    }

    (void) signal(SIGTERM, signal_handler);
    char name = argv[1][0];
    char moving;
    int msgid = -1;	/* Message Queue ID */
    int msgid2 = -1;
    int vtype = 0;
    int mtype = 1;
    int ret;
    int i;

    if (name <'A' || name > 'Z') //Wenn der Wagenname kein Großbuchstabe war
    {
        return -1;
    }


    if((msgid = msgget(KEY,PERM))==-1 ) //Message Queue erstellen
    {
        return -1;
      //return EXIT_FAILURE;
    }

    if((msgid2 = msgget(KEY2, PERM)) == -1)
    {
        return -1;
    }

    i = sending(name, msgid, vtype);

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
            case 'N' : //Queue schicken mit dem Move
            case 'O' :
            case 'S' :
            case 'W' :
            case 'T' : ret = sending(moving, msgid, mtype); break;
            default: cout << "Ungültiger Move" << endl;
        }

        if(ret == -1)
        {
            cout << "Error while movement" << endl;
        }
    }

    return 0;
}

int sending(char x, int messid, int type)
{

    //Nachricht verschicken
    message_t msg;	/* Buffer fuer Message */
   msg.mText = x;
   msg.mType = type;
   if (msgsnd(messid,&msg,sizeof(msg)-sizeof(long), 0) == -1) //Nachricht in Message Queue schreiben
   {
         //return EXIT_FAILURE;
         return -1;
   }
    return 0;

}
