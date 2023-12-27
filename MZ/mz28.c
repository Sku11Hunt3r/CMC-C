#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/wait.h>
#define ALF_LEN 26
#define DIG_KOL 10
int main(){
	key_t key;
	pid_t pid1, pid2, pid3;
	int shmid, semid, n, ff;
	char* shmaddr;
	struct sembuf P1 = {0, -1, 0}, V1 = {0, 1, 0};
	struct sembuf P2 = {1, -1, 0}, V2 = {1, 1, 0};
	struct sembuf P3 = {2, -1, 0}, V3 = {2, 1, 0};
	struct sembuf P4 = {3, -1, 0}, V4 = {3, 1, 0};
	
	ff = creat("a.txt",0664);
	if(ff < 0){
		perror("creat");
		return 1;
	}
	key = ftok("a.txt", 'k');
	if(key < 0){
		fprintf(stderr, "key error");
		return 1;
	}
	scanf("%d", &n);	
	shmid = shmget(key, (n+1) * sizeof(char), IPC_CREAT|0666);
	if(shmid < 0){
		fprintf(stderr, "shmget error");
		return 1;
	}
	semid = semget(key, 4, IPC_CREAT|0666);
        if(semid < 0){
		fprintf(stderr, "semget error");
		return 1;
	}
	semctl(semid, 0, SETVAL, 0);
	semctl(semid, 1, SETVAL, 0);
	semctl(semid, 2, SETVAL, 0);
	semctl(semid, 3, SETVAL, 0);
	if((pid1 = fork()) < 0){
		fprintf(stderr,"Can't start process 1");
		return 1;
	}
	if(!pid1){//son1
		shmaddr = shmat(shmid, NULL, 0);
		int letter = 0;
		if(shmaddr == (void*)-1){
			fprintf(stderr,"Can't attach shared memory");
			return 1;
		}
		while(1){
			semop(semid, &P1, 1);
			int i,pos;
			for(i = 0;i < (n+1); ++i){
				if(shmaddr[i] == '\0'){
					semop(semid,&V4,1);
					semop(semid,&P1,1);
				}
				if(shmaddr[i] == '.'){
					pos = i;
					break;
				}
			}
			shmaddr[pos] = ('a' + (letter % ALF_LEN));
			++letter;
			semop(semid,&V2,1);
		}
	}
	else{
		if((pid2 = fork()) < 0){
			fprintf(stderr,"Can't start process 2");
			return 1;
		}
		if(!pid2){//son2
			shmaddr = shmat(shmid, NULL, 0);
	                int digit = 0;
			if(shmaddr == (void*)-1){
	                        fprintf(stderr,"Can't attach shared memory");
	                        return 1;
	                }
	                while(1){
	                        semop(semid, &P2, 1);
				int i,pos;
	                        for(i = 0;i < (n+1); ++i){
		                   	if(shmaddr[i] == '\0'){
	                                        semop(semid,&V4,1);
						semop(semid,&P2,1);
	                                }
	                                if(shmaddr[i] == '.'){
	                                        pos = i;
        	                                break;
			                }
		                }
		                shmaddr[pos] = ('0' + (digit % DIG_KOL));
				++digit;
				semop(semid,&V3,1);
			}
		}
		else{
			if((pid3 = fork()) < 0){//son3
				fprintf(stderr,"Can't start process 3");
				return 1;
			}
			if(!pid3){
				shmaddr = shmat(shmid, NULL, 0);
		                int letter = 0;
				if(shmaddr == (void*)-1){
		                        fprintf(stderr,"Can't attach shared memory");
		                        return 1;
		                }
		                while(1){
		                        semop(semid, &P3, 1);
					int i,pos;
	                   		for(i = 0;i < (n+1); ++i){
		                                if(shmaddr[i] == '\0'){
		                                        semop(semid,&V4,1);
							semop(semid,&P3,1);
		                                }
		                                if(shmaddr[i] == '.'){
	        	                                pos = i;
		                                        break;
		                                }
		                        }
		                        shmaddr[pos] = ('A' + (letter % ALF_LEN));
					++letter;
					semop(semid,&V1,1);
		                }
			}
			else{//father
				shmaddr = shmat(shmid, NULL, 0);
		                int i;
				if(shmaddr == (void*)-1){
		                        fprintf(stderr,"Can't attach shared memory");
		                        return 1;
				}
				for(i = 0;i < n; ++i) shmaddr[i] = '.';
				shmaddr[n] = '\0';
				semop(semid, &V1, 1);
				semop(semid, &P4, 1);
				kill(pid1,SIGKILL);
				kill(pid2,SIGKILL);
				kill(pid3,SIGKILL);
				wait(NULL);
				wait(NULL);
				wait(NULL);
				printf("%s\n", shmaddr);
				semctl(semid, 0, IPC_RMID, NULL);
       				shmctl(shmid, IPC_RMID, NULL);	
			}
		}
	}
	return 0;
}
