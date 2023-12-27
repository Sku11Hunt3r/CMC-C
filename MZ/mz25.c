#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/wait.h>
volatile int flag1 = 0;
volatile int end = 0;
void SigHandlr1(int s){
	signal(SIGUSR1,SigHandlr1);
	flag1 = 1;
}
void Sigstop(int s){
	end = 1;
}
void do_work(char* str,int* cnt1,int* cnt2,pid_t pid,int* fd){
	if(!flag1) pause();
	flag1 = 0;
	read(fd[0],cnt1,sizeof(int));
	read(fd[0],cnt2,sizeof(int));
	if(end) return;
	printf("%s %d %d\n",str,(*cnt1)--,(*cnt2)++);
	fflush(stdout);
	write(fd[1],cnt1,sizeof(int));
	write(fd[1],cnt2,sizeof(int));
	kill(pid,SIGUSR1);
}
int main(){
	pid_t pid1,pid2;
	int fd[2];
	int cnt1,cnt2;
	if(pipe(fd) < 0){
		printf("Can't create pipe");
		fflush(stdout);
		return 0;
	}
	signal(SIGUSR1,SigHandlr1);
	signal(SIGUSR2,Sigstop);
	scanf("%d %d",&cnt1,&cnt2);
	if((pid1 = fork()) < 0){
		printf("Can't start process 1");
		fflush(stdout);
		return 0;
	}
	if(!pid1){//son1
		pid_t ppid = getppid();
		while(cnt1 >= cnt2) do_work("son1",&cnt1,&cnt2,ppid,fd);
		kill(ppid,SIGUSR2);
		if(!end){
			write(fd[1],&cnt1,sizeof(int));
			write(fd[1],&cnt2,sizeof(int));
			write(fd[1],&cnt1,sizeof(int));
			write(fd[1],&cnt2,sizeof(int));
		}
		kill(ppid,SIGUSR1);
		close(fd[1]);
		close(fd[0]);
	}
	else{
		if((pid2 = fork()) < 0){
			printf("Can't create process 2");
			fflush(stdout);
			return 0;
		}
		if(!pid2){//son2
			while(cnt1 >= cnt2) do_work("son2",&cnt1,&cnt2,pid1,fd);
			kill(pid1,SIGUSR2);
			if(!end){
				write(fd[1],&cnt1,sizeof(int));
				write(fd[1],&cnt2,sizeof(int));
				write(fd[1],&cnt1,sizeof(int));
				write(fd[1],&cnt2,sizeof(int));
			}
			kill(pid1,SIGUSR1);
			close(fd[0]);
			close(fd[1]);
		}
		else{//father
			write(fd[1],&cnt1,sizeof(int));
			write(fd[1],&cnt2,sizeof(int));
			kill(pid2,SIGUSR1);
			while(cnt1 >= cnt2) do_work("father",&cnt1,&cnt2,pid2,fd);
			kill(pid2,SIGUSR2);
			if(!end){
				write(fd[1],&cnt1,sizeof(int));
				write(fd[1],&cnt2,sizeof(int));
				write(fd[1],&cnt1,sizeof(int));
				write(fd[1],&cnt2,sizeof(int));
			}
			kill(pid2,SIGUSR1);
			close(fd[0]);
			close(fd[1]);
			wait(NULL);
			wait(NULL);
		}
	}
	return 0;
}
