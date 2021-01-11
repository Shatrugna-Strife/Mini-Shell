#include"header.h"

int getArgTokens(char*,struct state*);
void execute(struct state*, int );

//int main(){
//	while(1){
//		char buff[MAX_BUFF];
//		struct state present;
//		scanf("%[^\n]%*c",buff);
//		//printf("%s %ld\n",buff,strlen(buff));
//		int total = getArgTokens(buff, &present);
//		for(int i = 0;i<total;i++){
//			if(present.arg[i] == NULL){
//				printf(" NULL ");
//				continue;
//			}
//			printf("%s ", present.arg[i]);
//		}
//		printf(" %s %d %d\n", present.delim, total, present.num);
//	}
//}

int getArgTokens(char * buf, struct state*curr){
	char c;
	short index = 0;
	char temp[200];
	int t = 0;
	int tot = 20000;
	int td = 0;
	int totd = 200;
	int fd;//
	char**path;//
	int n = strlen(buf);
	curr->num = 0;
	curr->arg = (char**)malloc(tot*sizeof(char*));
	curr->delim = (char*)malloc(totd*sizeof(char));
	for(int i = 0;i<n;i++){
		switch(buf[i]){
			case '\\':
				i++;
				temp[index] = buf[i];
				index++;
				continue;
			case ' ':
				if(index>0){
					temp[index] = '\0';
					char *tmp = (char*)malloc((strlen(temp)+1)*sizeof(char));
					strcpy(tmp,temp);
					index = 0;
					curr->arg[t] = tmp;
					t++;
					if(t>tot){
						tot = (3*tot)/2;
						curr->arg = realloc(curr->arg, tot*sizeof(char*));
					}
					continue;
				}
				continue;
			case '|':
				if(index>0){
					temp[index] = '\0';
					char *tmp = (char*)malloc((strlen(temp)+1)*sizeof(char));
					strcpy(tmp,temp);
					index = 0;
					curr->arg[t] = tmp;
					t++;
					if(t>tot){
						tot = (3*tot)/2;
						curr->arg = realloc(curr->arg, tot*sizeof(char*));
					}
				}
				if(buf[i+1] == '|'){
					i = i+1;
					curr->delim[td] = 'o';
					if(buf[i+1] == '|'){
						i = i+1;
						curr->delim[td] = 't';
					}
				}else{
					curr->delim[td] = 'p';
				}
				td++;
				if(td>totd){
					totd = (3*totd)/2;
					curr->delim = realloc(curr->delim, totd*sizeof(char));
				}
				curr->arg[t] = NULL;
				curr->num++;
				t++;
				if(t>tot){
					tot = (3*tot)/2;
					curr->arg = realloc(curr->arg, tot*sizeof(char*));
				}
				continue;
			case '&':
				if(index>0){
					temp[index] = '\0';
					char *tmp = (char*)malloc((strlen(temp)+1)*sizeof(char));
					strcpy(tmp,temp);
					index = 0;
					curr->arg[t] = tmp;
					t++;
					if(t>tot){
						tot = (3*tot)/2;
						curr->arg = realloc(curr->arg, tot*sizeof(char*));
					}
				}
				if(buf[i+1] == '&'){
					i=i+1;
					curr->delim[td] = 'a';
				}else{
					printf("There is no implementation of & operator");
					continue;
				}
				td++;
				curr->num++;
				if(td>totd){
					totd = (3*totd)/2;
					curr->delim = realloc(curr->delim, totd*sizeof(char));
				}
				t++;
				if(t>tot){
					tot = (3*tot)/2;
					curr->arg = realloc(curr->arg, tot*sizeof(char*));
				}
				continue;
			case ',':
				if(index>0){
					temp[index] = '\0';
					char *tmp = (char*)malloc((strlen(temp)+1)*sizeof(char));
					strcpy(tmp,temp);
					index = 0;
					curr->arg[t] = tmp;
					t++;
					if(t>tot){
						tot = (3*tot)/2;
						curr->arg = realloc(curr->arg, tot*sizeof(char*));
					}
				}
				curr->delim[td] = 'c';
				td++;
				curr->num++;
				if(td>totd){
					totd = (3*totd)/2;
					curr->delim = realloc(curr->delim, totd*sizeof(char));
				}
				curr->arg[t] = NULL;
				t++;
				if(t>tot){
					tot = (3*tot)/2;
					curr->arg = realloc(curr->arg, tot*sizeof(char*));
				}
				continue;
			case '>':
				if(index>0){
					temp[index] = '\0';
					char *tmp = (char*)malloc((strlen(temp)+1)*sizeof(char));
					strcpy(tmp,temp);
					index = 0;
					curr->arg[t] = tmp;
					t++;
					if(t>tot){
						tot = (3*tot)/2;
						curr->arg = realloc(curr->arg, tot*sizeof(char*));
					}
				}
				if(buf[i+1] == '>'){
					i = i+1;
					curr->delim[td] = 'h';
				}else{
					curr->delim[td] = 'g';
				}
				td++;
				curr->num++;
				if(td>totd){
					totd = (3*totd)/2;
					curr->delim = realloc(curr->delim, totd*sizeof(char));
				}
				curr->arg[t] = NULL;
				t++;
				if(t>tot){
					tot = (3*tot)/2;
					curr->arg = realloc(curr->arg, tot*sizeof(char*));
				}
				continue;
			case '<':
				if(index>0){
					temp[index] = '\0';
					char *tmp = (char*)malloc((strlen(temp)+1)*sizeof(char));
					strcpy(tmp,temp);
					index = 0;
					curr->arg[t] = tmp;
					t++;
					if(t>tot){
						tot = (3*tot)/2;
						curr->arg = realloc(curr->arg, tot*sizeof(char*));
					}
				}
				curr->delim[td] = 'l';
				td++;
				curr->num++;
				if(td>totd){
					totd = (3*totd)/2;
					curr->delim = realloc(curr->delim, totd*sizeof(char));
				}
				curr->arg[t] = NULL;
				t++;
				if(t>tot){
					tot = (3*tot)/2;
					curr->arg = realloc(curr->arg, tot*sizeof(char*));
				}
				continue;
			default:
				;
		}
		temp[index] = buf[i];
		index++;
				
	}
	if(index>0){
		temp[index] = '\0';
		char *tmp = (char*)malloc((strlen(temp)+1)*sizeof(char));
		strcpy(tmp,temp);
		index = 0;
		curr->arg[t] = tmp;
		t++;
		if(t>tot){
		tot = (3*tot)/2;
			curr->arg = realloc(curr->arg, tot*sizeof(char*));
		}
	}
	curr->arg[t] = NULL;
	t++;
	curr->arg = realloc(curr->arg, t*sizeof(char*));
	curr->delim = realloc(curr->delim, td*sizeof(char));
	return t;
}
