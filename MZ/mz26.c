#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/wait.h>
int main(){
	pid_t pid1,pid2;
	int fd[2];
	int cnt1,cnt2,ff,semid,flag = 0;
	key_t key;
	if(pipe(fd) < 0){
		printf("Can't create pipe");
		fflush(stdout);
		return 0;
	}
	struct sembuf P1={0,-1,0},V1 = {0,1,0};
	struct sembuf P2={1,-1,0},V2 = {1,1,0};
	struct sembuf P3={2,-1,0},V3 = {2,1,0};
	struct sembuf P4={3,-1,0},V4 = {3,1,0};
	ff = creat("a.txt",0664);
	if(ff < 0){
		perror("creat");
		return 1;
	}
	key = ftok("a.txt",'k');
	if(key < 0){
		perror("ftok");
		return 1;
	}
	close(ff);
	semid = semget(key,4,IPC_CREAT|0666);
	if(semid < 0){
		perror("semget");
		return 1;
	}
	semctl(semid,0,SETVAL,0);
	semctl(semid,1,SETVAL,1);
	semctl(semid,2,SETVAL,0);
	semctl(semid,3,SETVAL,0);
	scanf("%d %d",&cnt1,&cnt2);
	if((pid2 = fork()) < 0){
		printf("Can't start process 2");
		fflush(stdout);
		return 0;
	}
	if(!pid2){//son2
		while(cnt1 >= cnt2){
			semop(semid,&P3,1);
			read(fd[0],&cnt1,sizeof(int));
			read(fd[0],&cnt2,sizeof(int));
			if(cnt1 >= cnt2){ 
				printf("son2 %d %d\n",cnt1--,cnt2++);
				fflush(stdout);
			}
			write(fd[1],&cnt1,sizeof(int));
			write(fd[1],&cnt2,sizeof(int));
			semop(semid,&V4,1);
		}
		close(fd[0]);
		close(fd[1]);
		return 0;
	}
	else{
		if((pid1 = fork()) < 0){
			printf("Can't create process 1");
			fflush(stdout);
			return 0;
		}
		if(!pid1){//son1
			while(cnt1 >= cnt2){
                        	semop(semid,&P2,1);
                        	read(fd[0],&cnt1,sizeof(int));
                        	read(fd[0],&cnt2,sizeof(int));
                        	if(cnt1 >= cnt2){
					printf("son1 %d %d\n",cnt1--,cnt2++);
                        		fflush(stdout);
				}
				write(fd[1],&cnt1,sizeof(int));
                        	write(fd[1],&cnt2,sizeof(int));
                        	semop(semid,&V1,1);
                	}
                	close(fd[0]);
                	close(fd[1]);
                	return 0;
		}
		else{//father
			if((cnt1-cnt2)%4 >= 2) flag = 1;
			write(fd[1],&cnt1,sizeof(int));
                        write(fd[1],&cnt2,sizeof(int));
			while(cnt1 >= cnt2){
                        	semop(semid,&P1,1);
                        	read(fd[0],&cnt1,sizeof(int));
                        	read(fd[0],&cnt2,sizeof(int));
                        	if(cnt1 >= cnt2){
					printf("father %d %d\n",cnt1--,cnt2++);
                   			fflush(stdout);
				}
				write(fd[1],&cnt1,sizeof(int));
                       		write(fd[1],&cnt2,sizeof(int));
                        	semop(semid,&V3,1);
				semop(semid,&P4,1);
                                read(fd[0],&cnt1,sizeof(int));
                                read(fd[0],&cnt2,sizeof(int));
                                if(cnt1 >= cnt2){
                                        printf("father %d %d\n",cnt1--,cnt2++);
                                        fflush(stdout);
                                }
                                write(fd[1],&cnt1,sizeof(int));
                                write(fd[1],&cnt2,sizeof(int));
                                semop(semid,&V2,1);
                	}
			if(flag){
				semop(semid,&P1,1);
				read(fd[0],&cnt1,sizeof(int));
                        	read(fd[0],&cnt2,sizeof(int));
                        	if(cnt1 >= cnt2){
                                	printf("father %d %d\n",cnt1--,cnt2++);
                                	fflush(stdout);
                        	}
                        	write(fd[1],&cnt1,sizeof(int));
                        	write(fd[1],&cnt2,sizeof(int));
                        	semop(semid,&V3,1);
			}
                 	close(fd[0]);
                	close(fd[1]);
                	wait(NULL);
			wait(NULL);
			semctl(semid,0,IPC_RMID,0);
			return 0;
		}
	}
}
