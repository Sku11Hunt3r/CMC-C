#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
int main(int argc,char** argv){
	int i,j,next[2],k=0;
	pid_t pid1,pid2,pid3;
	int fd1[2],fd2[2];
	for(i = 1;i < argc;++i){
		if(!(strcmp(argv[i],"|"))){
			next[k] = i;
			++k;
			argv[i] = NULL;
		}
	}
	i = next[0]+1;
	j = next[1]+1;
	if(pipe(fd1) < 0){
		perror("pipe1");
		return 1;
	}
	if((pid1 = fork()) < 0){
		perror("fork1");
		return 1;
	}
	if(!pid1){
		dup2(fd1[1],1);
		close(fd1[0]);
		close(fd1[1]);
		close(fd2[0]);
		close(fd2[1]);
		execvp(argv[1],argv + 1);
		fprintf(stderr,"Can't start process 1");
		return 0;
	}
	else{
		//wait(NULL);
		if(pipe(fd2) < 0){
                	perror("pipe2");
                	return 1;
        	}
        	if((pid2 = fork()) < 0){
                	perror("fork2");
                	return 1;
        	}
        	if(!pid2){
			dup2(fd1[0],0);
                	dup2(fd2[1],1);
                	close(fd1[0]);
                	close(fd1[1]);
			close(fd2[0]);
			close(fd2[1]);
                	execvp(argv[i],argv+i);
                	fprintf(stderr,"Can't start process 2");
			return 0;
        	}
		else{
			//wait(NULL);
			if((pid3 = fork()) < 0){
                		perror("fork3");
                		return 1;
        		}
        		if(!pid3){
				dup2(fd2[0],0);
                		close(fd2[0]);
                		close(fd2[1]);
				close(fd1[0]);
				close(fd1[1]);
				execvp(argv[j],argv+j);
                		fprintf(stderr,"Can't start process 3");
				return 0;
        		}
		}
	}
	close(fd1[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd2[1]);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	return 0;
}
