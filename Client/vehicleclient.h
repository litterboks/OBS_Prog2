#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>

#define KEY 14049
#define KEY2 14062	/* eindeutiger Key z.B. Matrikelnummer */
#define PERM 0660

#define MAX_DATA 255

typedef struct
{
	long mType;
	int mText;
} message_t;
