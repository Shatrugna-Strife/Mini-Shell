#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdbool.h>
#include <fcntl.h>
#include<signal.h>
#include<assert.h>
#define MAX_BUFF 32768

struct state{
	char**arg;
	char*delim;
	int num;
};

extern int getPath(char**);
extern int getPathTokens(int , char**);
extern int getArgTokens(char*,struct state*);
extern void execute(struct state*, int );


