#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>
#define M 10
#define N 10
#define K 50
enum SYNT_ERROR{OK = 0,ERR = 1};
enum SYNT_ERROR synt_err;
enum IN_QOUTS{IN = 1,OUT = 0};
enum IN_QOUTS in = OUT;
void skip_space(int* c){
        while(*c == ' ') *c = getchar();
}

void close_pipes(int* fd){
        close(fd[0]);
        close(fd[1]);
}

void mk_proc(char*** arr, int i, int j, int fd[2]){
	/*pid_t pid;
        int* fd2 = (int*)malloc(2*sizeof(int));
	//int fd2[2];
        if(i == 0) return;
        printf("%s\n",arr[j - i][0]);
	if((pid1 = fork()) < 0){
		fprintf(stderr,"Can't make process");
                return;
	}
	if(!pid1){
		if(i == j){
                        pipe(fd2);
                        if(pipe(fd2) < 0){
		                fprintf(stderr,"Can't create pipe\n");
		                return;
	                }
                        dup2(fd2[1], 1);
                        close(fd2[0]);
                        close(fd2[1]);
                        execvp(arr[j - i][0], arr[j - i]);
                        fprintf(stderr,"Command %s not found\n", arr[j - i][0]);
                        fflush(stderr);
                        return;
                }
                else{
                        if(i == 1){
                                dup2(fd[0], 0);
                                close(fd[0]);
                                close(fd[1]);
                                execvp(arr[j - i][0], arr[j - i]);
                                fprintf(stderr,"Command %s not found\n", arr[j - i][0]);
                                fflush(stderr);
                                return;
                        }
                        else{
                                pipe(fd2);
                                if(pipe(fd2) < 0){
		                        fprintf(stderr,"Can't create pipe\n");
		                        return;
	                        }
                                dup2(fd[0], 0);
                                //dup2(fd2[1], 1);
                                close(fd[0]);
                                close(fd[1]);
                                close(fd2[0]);
                                close(fd2[1]);
                                execvp(arr[j - i][0], arr[j - i]);
                                fprintf(stderr,"Command %s not found\n", arr[j - i][0]);
                                fflush(stderr);
                                return;
                        }
                }
	}
        if(i != j){
                close(fd[0]);
                close(fd[1]);
        }
        close(fd2[0]);
        close(fd2[1]);
        printf("IN WAIT\n");
        wait(NULL);
        mk_proc(arr,i-1,j,fd2);
        return;*/
        if(i == 0) return;
        pid_t pid;
        int fd_out[2], status;
        if(pipe(fd_out) < 0){
                perror("Pipe error\n");
                fflush(stderr);
        }
        if((pid = fork()) < 0){
                if(j != i) close_pipes(fd);
                close_pipes(fd_out);
                perror("Fork error\n");
                fflush(stderr);
                exit(0);
        }
        else{
                if(!pid){
                     if(i != j) dup2(fd[0], 0);
                        if(i != 1) dup2(fd_out[1], 1);
                        if(i != j) close_pipes(fd);
                        close_pipes(fd_out);
                        execvp(arr[j-i][0], arr[j-i]);
                        fprintf(stderr, "Command %s not found\n", arr[j-i][0]);
                        fflush(stderr);
                        exit(-1);
                
                }
                else{
                        if(j != i) close_pipes(fd);
                        wait(&status);
                        if(WEXITSTATUS(status) || i == 1){
                                if(j != i) close_pipes(fd);
                                close_pipes(fd_out);
                                exit(0);
                        }
                        mk_proc(arr,i-1,j,fd_out);
                        close_pipes(fd_out);
                        if(j != i) close_pipes(fd);
                        exit(0);
                }
        }
}

char*** str2arr(int* kol_arg){
        int m = 0,n = 0,k = 0, kolm = 2, koln = 2, kolk = 2,c;
        char prev_c = '\n';
        char*** arr = (char***)malloc(M*sizeof(char**));
        if(arr == NULL) return NULL;
        c = getchar();
        while(c != '\n'){
                n = 0;
                k = 0;
                char** arr2 = (char**)malloc(N*sizeof(char*));
                if(arr2 == NULL) return NULL;
                while(c != '\n' && c != '|'){
                        k = 0;
                        char* arr3 = (char*)malloc(K*sizeof(char));
                        if(arr3 == NULL) return NULL;
                        while(c != '\n' && c != '|' && c != ' '){
                                if(c == '\'' || c == '\"'){
                                        in = IN;
                                        if(c == '\''){
                                                c = getchar();
                                                while(c != '\'' && c != '\n' && c != '|'){
                                                        arr3[k + (kolk-2)*K] = c;
                                                        ++k;
                                                        c = getchar();
                                                        if(k == K - 1){
                                                                arr3 = (char*)realloc(arr3,kolk*K*sizeof(char));
                                                                if(arr3 == NULL) return NULL;
                                                                ++kolk;
                                                                k = -1;
                                                        }
                                                }
                                        }
                                        else{
                                                c = getchar();
                                                while(c != '\"' && c != '\n' && c != '|'){
                                                        arr3[k + (kolk-2)*K] = c;
                                                        ++k;
                                                        c = getchar();
                                                        if(k == K - 1){
                                                                arr3 = (char*)realloc(arr3,kolk*K*sizeof(char));
                                                                if(arr3 == NULL) return NULL;
                                                                ++kolk;
                                                                k = -1;
                                                        }
                                                }
                                        }
                                }
                                if(c == '\'' || c == '\"') in = OUT;
                                else{
                                        arr3[k + (kolk - 2)*K] = c;
                                        ++k;
                                }
                                if(c != '|' && c != '\n') c = getchar();
                                if(k == K - 1){
                                        arr3 = (char*)realloc(arr3,kolk*K*sizeof(char));
                                        if(arr3 == NULL) return NULL;
                                        ++kolk;
                                        k = -1;
                                }
                        }
                        if(in == IN){
                                synt_err = ERR;
                                return NULL;
 			}
                        skip_space(&c);
                        arr3[(kolk - 2)*K + k] = '\0';
                        kolk = 2;
                        arr2[n + (koln - 2)*N] = arr3;
                        ++n;
                        if(n == N -1){
                                arr2 = (char**)realloc(arr2,koln*N*sizeof(char*));
                                if(arr2 == NULL) return NULL;
                                ++koln;
                                n = -1;
                        }
                }
                if(kolk == 2 && k == 0){
                        synt_err = ERR;
                        return NULL;
                }
                prev_c = c;
                arr2[(koln - 2)*N +n] = NULL;
                koln = 2;
                arr[m + (kolm - 2)*M] = arr2;
                ++m;
                if(m == M-1){
                        arr = (char***)realloc(arr,kolm*M*sizeof(char**));
                        if(arr == NULL) return 0;
                        ++kolm;
                        m = -1;
                }
                if(c == '|' && (arr[m - 1 + (kolm - 2)*M] == NULL)){
                        synt_err = ERR;
                        return NULL;
                }
                if(c != '\n') c = getchar();
                skip_space(&c);
        }
        if((kolk == 2 && k == 0) || prev_c == '|'){
                synt_err = ERR;
                return NULL;
        }
        arr[(kolm - 2)*M + m] = NULL;
        *kol_arg = (kolm - 2)*M + m;
        return arr;
}

int main(){
        int i = 0,kol_arg;
        synt_err = OK;
        char*** arr = str2arr(&kol_arg);
        if(synt_err == ERR){
                fprintf(stderr,"Syntax error\n");
                return 0;
        }
        char*** p = arr;
        /*while(p[i] != NULL){
                int j = 0;
                while(p[i][j] != NULL){
                        printf("%d %d %s ",i,j,p[i][j]);
                        ++j;
                }
                ++i;
                printf("\n");
        }
        p = arr;*/
        while(p[i] != NULL){
                if(p[i][0] == NULL){
                        fprintf(stderr,"Syntax error\n");
                        return 0;
                }
                ++i;
        }
        if(kol_arg == 1){
                execvp(arr[0][0], arr[0]);
                fprintf(stderr,"Command %s not found\n", arr[0][0]);
        }
        else{
                mk_proc(arr,kol_arg,kol_arg,NULL);
        }
        return 0;
}