#include"header.h"

int getPath(char**);
int getPathTokens(int , char**);

//int main(){
//	char **pv;
//	pv = (char**)malloc(20000*sizeof(char*));
//	int pt = getPath(pv);
//	pv = realloc(pv, sizeof(char*)*pt);
//	for(int i = 0;i<pt;i++)printf("%s\n",pv[i]);
//}

int getPath(char **p){
	int pip[2];
	pipe(pip);
	char t[1024];
	pid_t c = fork();
	if(c==0){
		dup2(pip[1], STDOUT_FILENO);
		char *arg[] = {"/usr/bin/ls","/usr/bin",NULL};
		execv(arg[0], arg);
	}
	int status;
	waitpid(c,&status, 1);
	close(pip[1]);
	int total = getPathTokens(pip[0], p);
	close(pip[0]);
	return total;
}

int getPathTokens(int fd, char** path){
	char c;
	short index = 0;
	char temp[200];
	int t = 0;
	int tot = 20000;
	while(read(fd,&c,1)!=0){
		if(c == '\n'){
			temp[index] = '\0';
			char *f = (char*)malloc((strlen(temp)+1)*sizeof(char));
			strcpy(f, temp);
			index = 0;
			path[t] = f;
			t++;
			//printf("%s\n",path[t-1]);
			continue;
		}
		temp[index] = c;
		index++;
	}
	if(index >0){
		temp[index] = '\0';
		char *f = (char*)malloc((strlen(temp)+1)*sizeof(char));			
		strcpy(f, temp);
		path[t] = f;
		index = 0;
		t++;
	}
	return t;		
}
