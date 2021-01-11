#include"header.h"

char **pv;
int pt;
char ** commands;
int ct;

void sigint(){
	printf("\nLast ten commands\n");
	for(int i = ct;ct-i<10&&i>=0;i--)printf("%s\n",commands[i]);
	exit(0);
}
void sigquit(){
	char s = '\0';
	printf("Do you wanna quit y:");
	//while(s!='y'||s!='Y'||s!='n'||s!='N'){
		scanf("%c%*c",&s);
		if(s == 'y'||s=='Y')exit(0);
	//}
	//printf("yeah\n");
}

void execute_command(struct state*);
bool binarySearch(char**, int , int , char*);

int main(){
	/*
	struct sigaction sa = {{0}};
    sa.sa_handler = &my_signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) != 0)
    {
        fprintf(stderr, "sigaction error\n");
        return -1;
    }
	*/
	
	
	struct sigaction act;
    act.sa_handler = SIG_IGN;
    for(int i = 1 ; i < 65 ; i++) {
        //printf("i = %d\n", i);
        // 9 and 19 cannot be caught or ignored                                                                                                       
        // 32 and 33 do not exist                                                                                                                     
        if((i != SIGKILL) && (i != SIGSTOP) && (i!=SIGINT) && (i!=SIGQUIT) && (i != 32) && (i != 33)) {
            assert(sigaction(i, &act, NULL) == 0);
        }
    }
	
	
	
	signal(SIGINT,sigint);
	signal(SIGQUIT,sigquit);
	pv = (char**)malloc(20000*sizeof(char*));
	pt = getPath(pv);
	pv = realloc(pv, sizeof(char*)*pt);
	commands = (char**)malloc(20000*sizeof(char*));
	ct =-1;
	//for(int i = 0;i<pt;i++)printf("%s\n",pv[i]);
	while(1){
		char buff[MAX_BUFF];
		struct state present;
		//printf(">");
		scanf("%[^\n]%*c",buff);
		if(strlen(buff)==0){
			continue;
		}
		char*tm = (char*)malloc((strlen(buff)+1)*sizeof(char));
		strcpy(tm,buff);
		ct++;
		commands[ct] = tm;
		//printf("%s %ld\n",buff,strlen(buff));
		int total = getArgTokens(buff, &present);
		//for(int i = 0;i<total;i++){
			//if(present.arg[i] == NULL){
				//printf(" NULL ");
				//continue;
			//}
			//printf("%s ", present.arg[i]);
		//}
		//printf(" %s %d %d\n", present.delim, total, present.num);
		execute_command(&present);
		//printf("\n");
		//freemem();
	}
}

bool checkbin(struct state *temp, int x){
	bool c = binarySearch(pv,0,pt-1,temp->arg[x]);
	if(c==true){
		char tmp[50];
		strcpy(tmp,temp->arg[x]);
		temp->arg[x] = realloc(temp->arg[x], sizeof(char)*(strlen(tmp)+strlen("/usr/bin/")+1));
		strcpy(temp->arg[x],"/usr/bin/");
		strcat(temp->arg[x],tmp);
	}else printf("%s doesn't exist\n",temp->arg[x]);
	return c;
}

void execute_command(struct state* temp){
	int n = temp->num;
	int s = 0;
	
			
	if(n == 0){
		if(strcmp("cd",temp->arg[s])==0){
			if(temp->arg[s+1]==NULL){
				printf("Error cd\n");
				return;
			}
			if(temp->arg[s+2]!=NULL){
				printf("Too many arguments\n");
				return;
			}else{
				chdir(temp->arg[s+1]);
			}
			return;
		}
		if(!checkbin(temp, s)){
			//printf("ds\n");
			return;
		}
		if(fork()==0){
			execv(temp->arg[0], temp->arg);
		}
		int status;
		wait(&status);
		return;
	}
	
	int st = 0;
	int tot = n+1;
	int i =0;
	for(int i =0;i<n&&st<tot;){
		if(temp->delim[i] == 'a'){
			if(strcmp("cd",temp->arg[s])==0){
				if(temp->arg[s+1]==NULL){
					printf("Error cd\n");
					return;
				}
				if(temp->arg[s+2]!=NULL){
					printf("Too many arguments\n");
					return;
				}else{
					chdir(temp->arg[s+1]);
				}
				i++;
				s++;
				while(temp->arg[s]!=NULL)s++;
				s++;
				st++;
				//i+=1;
				//if(i<n&&temp->delim[i]!='a'){
				//	printf("only && operator after cd\n");
				//	return;
				//}
				//i++;
				continue;
			}	
			if(!checkbin(temp, s)){
				//printf("ds\n");
				return;
			}
			if(fork()==0){
				execv(temp->arg[s],temp->arg+s);
				printf("error in execution\n");
			}
			int status;
			wait(&status);
			i++;
			s++;
			while(temp->arg[s]!=NULL)s++;
			s++;
			st++;
			continue;
		}
		if(temp->delim[i] == 'g'||temp->delim[i] == 'h'){
			int next = s;
			while(temp->arg[next]!=NULL)next++;
			next++;
			if(temp->arg[next]==NULL||temp->arg[next+1]!=NULL){
				printf("Arguments Issue\n");
				return;
			}
			int fd;
			if(temp->delim[i] == 'g')fd = open(temp->arg[next], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if(temp->delim[i] == 'h')fd = open(temp->arg[next], O_WRONLY | O_APPEND | O_CREAT, 0644);
			if(fd==-1){
				printf("File error\n");
				return;
			}
			if(!checkbin(temp, s)){
				//printf("ds\n");
				return;
			}
			if(fork()==0){
				dup2(fd,STDOUT_FILENO);
				execv(temp->arg[s],temp->arg+s);
				printf("%s, error in execution\n", temp->arg[s]);
			}
			int status;
			wait(&status);
			close(fd);
			i+=1;
			if(i<n&&temp->delim[i]!='a'){
				printf("only && operator after <<,<,> operator\n");
				return;
			}
			i++;
			s++;
			while(temp->arg[s]!=NULL)s++;
			s++;
			s++;
			while(temp->arg[s]!=NULL)s++;
			s++;
			st+=2;
			continue;
		}
		//if(temp->delim[i] == 
		if(temp->delim[i] == 'l'){
			int next = s;
			while(temp->arg[next]!=NULL)next++;
			next++;
			if(temp->arg[next]==NULL||temp->arg[next+1]!=NULL){
				printf("Arguments Issue\n");
				return;
			}
			int fd;
			fd = open(temp->arg[next], O_RDONLY,0);
			if(fd==-1){
				printf("%s doesn't exists\n",temp->arg[next]);
				return;
			}
			//if(temp->delim[i] == 'h')fd = open(temp->arg[next], O_WRONLY | O_APPEND | O_CREAT, 0644);
			if(!checkbin(temp, s)){
				//printf("ds\n");
				return;
			}
			if(fork()==0){
				dup2(fd,STDIN_FILENO);
				execv(temp->arg[s],temp->arg+s);
				printf("%s, error in execution\n", temp->arg[s]);
			}
			int status;
			wait(&status);
			close(fd);
			i+=1;
			if(i<n&&temp->delim[i]!='a'){
				printf("only && operator after <<,<,> operator\n");
				return;
			}
			i++;
			//s++;
			while(temp->arg[s]!=NULL)s++;
			s++;
			//s++;
			while(temp->arg[s]!=NULL)s++;
			s++;
			st+=2;
			continue;
		}
		
		if(temp->delim[i] == 'p'){
			int ttp = 0;
			int j = i;
			while(j<n&&temp->delim[j]=='p'){
				j++;
				ttp++;
			}
			int pp[ttp][2];
			int k;
			for(k = 0;k<ttp;k++){
				if(pipe(pp[k])==-1)printf("pipefail\n");
				if(!checkbin(temp, s)){
					//printf("ds\n");
					return;
				}
				if(fork()==0){
					if(k>0&&k<ttp){
						dup2(pp[k-1][0],STDIN_FILENO);
					}
					if(dup2(pp[k][1],STDOUT_FILENO)==-1)printf("eeror\n");
					//printf("err %s\n",temp->arg[s]);
					execv(temp->arg[s],temp->arg+s);
					printf("error in execution %s\n",temp->arg[s]);
				}
				int status;
				wait(&status);
				if(k>0&&k<ttp){
					close(pp[k-1][0]);
				}
				close(pp[k][1]);
				//s++;
				while(temp->arg[s]!=NULL)s++;
				s++;
				st++;
				//printf("%d k\n",k);
			}
			//printf("%d %d  k\n",k,ttp);
			i+=ttp;
			
			
			
			
			if((i<n)&&temp->delim[i] == 'g'||temp->delim[i] == 'h'){
				int next = s;
				while(temp->arg[next]!=NULL)next++;
				next++;
				if(temp->arg[next]==NULL||temp->arg[next+1]!=NULL){
					printf("Arguments Issue\n");
					return;
				}
				int fd;
				if(temp->delim[i] == 'g')fd = open(temp->arg[next], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if(temp->delim[i] == 'h')fd = open(temp->arg[next], O_WRONLY | O_APPEND | O_CREAT, 0644);
				if(fd<-1){
					printf("File error\n");
					return;
				}
				if(!checkbin(temp, s)){
					//printf("ds\n");
					return;
				}
				if(fork()==0){
					dup2(pp[k-1][0],STDIN_FILENO);
					dup2(fd,STDOUT_FILENO);
					execv(temp->arg[s],temp->arg+s);
					printf("error in execution %s\n",temp->arg[s]);
				}
				int status;
				wait(&status);
				close(fd);
				i+=1;
				if(i<n&&temp->delim[i]!='a'){
					printf("only && operator after <<,<,> operator\n");
					return;
				}
				i++;
				//s++;
				while(temp->arg[s]!=NULL)s++;
				s++;
				st++;
				while(temp->arg[s]!=NULL)s++;
				s++;
				st++;
				continue;
			}
			
			
			
			
			if(!checkbin(temp, s)){
				//printf("ds\n");
				return;
			}
			if(fork()==0){
				dup2(pp[k-1][0],STDIN_FILENO);
				//printf("err %s\n",temp->arg[s]);
				execv(temp->arg[s],temp->arg+s);
				printf("error in execution %s\n",temp->arg[s]);
			}
			int status;
			wait(&status);
			close(pp[k-1][0]);
			//s++;
			while(temp->arg[s]!=NULL)s++;
			s++;
			st++;
			if(i<n&&temp->delim[i]!='a'){
				printf("only && >> > operator after | operator\n");
				return;
			}
			i++;
			continue;
		}
		if(temp->delim[i] == 'o'){
			int start = s;
			int pip1[2];
			int pip2[2];
			pipe(pip1);
			pipe(pip2);
			if(!checkbin(temp, s)){
				//printf("ds\n");
				return;
			}
			//for(int k=0;k<2;k++){
			//s++;
			while(temp->arg[s]!=NULL)s++;
			s++;
			st++;
		
			if(!checkbin(temp, s)){
				//printf("ds\n");
				return;
			}
			if(fork()==0){
				if(dup2(pip1[1],STDOUT_FILENO)==-1)printf("eeror\n");
				//printf("err %s\n",temp->arg[s]);
				execv(temp->arg[start],temp->arg+start);
				printf("error in execution %s\n",temp->arg[s]);
			}
			int status;
			wait(&status);
			close(pip1[1]);
			
			if(fork()==0){
				if(dup2(pip1[0],STDIN_FILENO)==-1)printf("eeror\n");
				//printf("err %s\n",temp->arg[s]);
				execv(temp->arg[s],temp->arg+s);
				printf("error in execution %s\n",temp->arg[s]);
			}
			//int status;
			wait(&status);
			close(pip1[0]);
			//s++;
			while(temp->arg[s]!=NULL)s++;
			s++;
			st++;
			
			if(!checkbin(temp, s)){
				//printf("ds\n");
				return;
			}
			if(fork()==0){
				if(dup2(pip2[1],STDOUT_FILENO)==-1)printf("eeror\n");
				//printf("err %s\n",temp->arg[s]);
				execv(temp->arg[start],temp->arg+start);
				printf("error in execution %s\n",temp->arg[s]);
			}
			//int status;
			wait(&status);
			close(pip2[1]);
			
			if(fork()==0){
				if(dup2(pip2[0],STDIN_FILENO)==-1)printf("eeror\n");
				//printf("err %s\n",temp->arg[s]);
				execv(temp->arg[s],temp->arg+s);
				printf("error in execution %s\n",temp->arg[s]);
			}
			//int status;
			wait(&status);
			close(pip2[0]);
			s++;
			while(temp->arg[s]!=NULL)s++;
			s++;
			st++;
			i+=2;
			if(i<n&&temp->delim[i]!='a'){
				printf("only && operator after || operator\n");
				return;
			}
			i++;
			continue;
		}
		if(temp->delim[i] == 't'){
			int start = s;
			int pip1[2];
			int pip2[2];
			int pip3[2];
			pipe(pip1);
			pipe(pip2);
			pipe(pip3);
			if(!checkbin(temp, s)){
				//printf("ds\n");
				return;
			}
			//for(int k=0;k<2;k++){
			//s++;
			while(temp->arg[s]!=NULL)s++;
			s++;
			st++;
		
			if(!checkbin(temp, s)){
				//printf("ds\n");
				return;
			}
			if(fork()==0){
				if(dup2(pip1[1],STDOUT_FILENO)==-1)printf("eeror\n");
				//printf("err %s\n",temp->arg[s]);
				execv(temp->arg[start],temp->arg+start);
				printf("error in execution %s\n",temp->arg[s]);
			}
			int status;
			wait(&status);
			close(pip1[1]);
			
			if(fork()==0){
				if(dup2(pip1[0],STDIN_FILENO)==-1)printf("eeror\n");
				//printf("err %s\n",temp->arg[s]);
				execv(temp->arg[s],temp->arg+s);
				printf("error in execution %s\n",temp->arg[s]);
			}
			//int status;
			wait(&status);
			close(pip1[0]);
			//s++;
			while(temp->arg[s]!=NULL)s++;
			s++;
			st++;
			
			if(!checkbin(temp, s)){
				//printf("ds\n");
				return;
			}
			if(fork()==0){
				if(dup2(pip2[1],STDOUT_FILENO)==-1)printf("eeror\n");
				//printf("err %s\n",temp->arg[s]);
				execv(temp->arg[start],temp->arg+start);
				printf("error in execution %s\n",temp->arg[s]);
			}
			//int status;
			wait(&status);
			close(pip2[1]);
			
			if(fork()==0){
				if(dup2(pip2[0],STDIN_FILENO)==-1)printf("eeror\n");
				//printf("err %s\n",temp->arg[s]);
				execv(temp->arg[s],temp->arg+s);
				printf("error in execution %s\n",temp->arg[s]);
			}
			//int status;
			wait(&status);
			close(pip2[0]);
			s++;
			while(temp->arg[s]!=NULL)s++;
			s++;
			st++;
			if(!checkbin(temp, s)){
				//printf("ds\n");
				return;
			}
			if(fork()==0){
				if(dup2(pip3[1],STDOUT_FILENO)==-1)printf("eeror\n");
				//printf("err %s\n",temp->arg[s]);
				execv(temp->arg[start],temp->arg+start);
				printf("error in execution %s\n",temp->arg[s]);
			}
			//int status;
			wait(&status);
			close(pip3[1]);
			
			if(fork()==0){
				if(dup2(pip3[0],STDIN_FILENO)==-1)printf("eeror\n");
				//printf("err %s\n",temp->arg[s]);
				execv(temp->arg[s],temp->arg+s);
				printf("error in execution %s\n",temp->arg[s]);
			}
			//int status;
			wait(&status);
			close(pip3[0]);
			s++;
			while(temp->arg[s]!=NULL)s++;
			s++;
			st++;
			i+=3;
			if(i<n&&temp->delim[i]!='a'){
				printf("only && operator after ||| operator\n");
				return;
			}
			i++;
			continue;
		}
			//}
		//if((i<n)&&temp->delim[i] == 'g'||temp->delim[i] == 'h'){
			//	int next = s;
				//while(temp->arg[next]!=NULL)next++;
				//next++;
				//if(temp->arg[next]==NULL||temp->arg[next+1]!=NULL){
					//printf("Arguments Issue\n");
					//return;
				//}
				//int fd;
				//if(temp->delim[i] == 'g')fd = open(temp->arg[next], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				//if(temp->delim[i] == 'h')fd = open(temp->arg[next], O_WRONLY | O_APPEND | O_CREAT, 0644);
				//if(fd<-1){
				//	printf("File error\n");
				//	return;
				//}
				//if(!checkbin(temp, s)){
					//printf("ds\n");
				//	return;
				//}
				//if(fork()==0){
				//	dup2(fd,STDOUT_FILENO);
				//	execv(temp->arg[s],temp->arg+s);
				//	printf("error in execution %s\n",temp->arg[s]);
				//}
				//int status;
				//wait(&status);
				//close(fd);
				//i+=1;
				//if(i<n&&temp->delim[i]!='a'){
				//	printf("only && operator after <<,<,> operator\n");
				//	return;
				//}
				//s++;
				//while(temp->arg[s]!=NULL)s++;
				//s++;
				//st++;
			//}
	}
	if(st<tot){
		if(strcmp("cd",temp->arg[s])==0){
			if(temp->arg[s+1]==NULL){
				printf("Error cd\n");
				return;
			}
			if(temp->arg[s+2]!=NULL){
				printf("Too many arguments\n");
				return;
			}else{
				chdir(temp->arg[s+1]);
			}
			return;
		}
		if(!checkbin(temp, s)){
			//printf("ds\n");
			return;
		}
		if(fork()==0){
			execv(temp->arg[s],temp->arg+s);
			printf("error in execution\n");
		}
		int status;
		wait(&status);
	}
}

bool binarySearch(char**data, int l, int r, char*find) 
{ 
    if (r >= l) { 
        int mid = l + (r - l) / 2; 
        if (strcmp(data[mid], find) == 0) 
            return true;  
        if (strcmp(data[mid], find) > 0) 
            return binarySearch(data, l, mid - 1, find); 
        return binarySearch(data, mid + 1, r, find); 
    }
    return false; 
}