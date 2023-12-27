#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/types.h>
struct mesbuf{
	long mtype;
	int netw_num;
};
int main(){
	int a,b,c,d,msgid,ff,mask = 1,i = 0;
	key_t key;
	pid_t pid1,pid2,pid3;
	struct mesbuf mes;
	ff = creat("a.txt",0666);
	if(ff < 0){
		perror("creat");
		return 1;
	}
	key = ftok("a.txt",12);
	if(key < 0){
		perror("ftok");
		return 1;
	}
	close(ff);
	msgid = msgget(key, IPC_CREAT|0666);
	if(msgid < 0){
		perror("msgget");
		return 1;
	}
	if((pid1 = fork()) < 0){
		printf("Can't start process 1");
		fflush(stdout);
		return 1;
	}
	if(!pid1){
		while(1){
			msgrcv(msgid,&mes,sizeof(int),1,0);
			if(mes.netw_num == -1) break;
			printf("A %x\n",mes.netw_num);
			fflush(stdout);
			mes.mtype = 4;
			msgsnd(msgid,&mes,sizeof(int),0);
		}
		return 0;
	}
	else{
		if((pid2 = fork()) < 0){
			printf("Can't start process 2");
			fflush(stdout);
			return 1;
		}
		if(!pid2){
			while(1){
				msgrcv(msgid,&mes,sizeof(int),2,0);
                        	if(mes.netw_num == -1) break;
                        	printf("B %x\n",mes.netw_num);
                        	fflush(stdout);
				mes.mtype = 4;
                        	msgsnd(msgid,&mes,sizeof(int),0);
			}
                        return 0;
		}
		else{
			if((pid3 = fork()) < 0){
				printf("Can't start process 2");
				fflush(stdout);
				return 0;
			}
			if(!pid3){
				while(1){
					msgrcv(msgid,&mes,sizeof(int),3,0);
                        		if(mes.netw_num == -1) break;
                        		printf("C %x\n",mes.netw_num);
                        		fflush(stdout);
					mes.mtype = 4;
                        		msgsnd(msgid,&mes,sizeof(int),0);
				}
                        	return 0;
			}
			else{
				while(scanf("%d.%d.%d.%d",&a,&b,&c,&d) == 4){
                			mes.netw_num = 0;
                			mask = 1 << 7;
                			if(!(mask & a)){
                        			mes.mtype = 1;
                        			for(i = 6; i >= 0;--i){
                                			int tmp;
                                			mask = 1 << i;
                                			tmp = mask & a;
                                			mes.netw_num += tmp;
                        			}
                			}
                			else{
                        			mask = 1 << 6;
                        			if(!(mask & a)){
                                			mes.mtype = 2;
                                			for(i = 5; i >= 0;--i){
                                        			int tmp;
                                        			mask = 1 << i;
                                        			tmp = mask & a;
                                        			mes.netw_num += (tmp << 8);
                                			}
                                			for(i = 7; i >= 0;--i){
                                        			int tmp;
                                        			mask = 1 << i;
                                        			tmp = mask & b;
                                        			mes.netw_num += tmp;
                                			}
                        			}
                        			else{
                                			mes.mtype = 3;
                                			for(i = 4; i >= 0;--i){
                                        			int tmp;
                                        			mask = 1 << i;
                                        			tmp = mask & a;
                                        			mes.netw_num += (tmp << 16);
                                			}
                                			for(i = 7; i >= 0;--i){
                                        			int tmp;
                                        			mask = 1 << i;
                                        			tmp = mask & b;
                                        			mes.netw_num += (tmp << 8);
                                			}
                                			for(i = 7; i >= 0;--i){
                                        			int tmp;
                                        			mask = 1 << i;
                                        			tmp = mask & c;
                                        			mes.netw_num += tmp;
                                			}
                        			}
                			}
                			msgsnd(msgid,&mes,sizeof(int),0);
					msgrcv(msgid,&mes,sizeof(int),4,0);
				}
				mes.mtype = 1;
				mes.netw_num = -1;
				msgsnd(msgid,&mes,sizeof(int),0);
				mes.mtype = 2;
                                mes.netw_num = -1;
                                msgsnd(msgid,&mes,sizeof(int),0);
				mes.mtype = 3;
                                mes.netw_num = -1;
                                msgsnd(msgid,&mes,sizeof(int),0);
				wait(NULL);
				wait(NULL);
				wait(NULL);
				msgctl(msgid,IPC_RMID,NULL);
			}
		}
	}
	return 0;
}
