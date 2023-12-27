#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#define N 13
#define STRLEN 50
#define NEWLINE "\n"
int main(int argc, char** argv){
        if(argc != 3){
                fprintf(stderr,"Error\n");
                return 0;
        }
        if(argv[1] == NULL || argv[2] == NULL){
                fprintf(stderr,"Error\n");
                return 0;
        }
        int fd_in,fd_out,left = STRLEN,tmp;
        char str[N],c;
        fd_in = open(argv[1],O_RDONLY,0644);
	if(fd_in < 0){
		fprintf(stderr,"Error\n");
		return 0;
	}
        fd_out = creat(argv[2],0644);
        if(fd_out < 0){
		fprintf(stderr,"Error\n");
		close(fd_in);
		return 0;
	}
	while((tmp = read(fd_in,str,N))/* == N && left > 0*/){
                //printf("NEWSTR:%d %s\n",left,str);
                int i = 0;
                c = str[i];
                while(/*c != EOF &&*/ i < tmp && left > 0){
                        c = str[i];
                        --left;
                        ++i;
                       	if(c == '\n') left = STRLEN;
                }
                        //str[i] = '\0';
                        //printf("%d\n",i);
                if(left == 0){
                        char* new_str;
                        int tmp2,j;
                        write(fd_out,str,i);
                        i = 0;
				//printf("%s\n",str);
			while(str[i] != '\n' && i < tmp) ++i;
                        //printf("LEFTAFTI:%s\n",str+i);
                                //printf("%d %d\n",i,tmp);
			if(i == tmp){
                               while(c != '\n'){
                                        tmp2 = read(fd_in,str,N);
                                                 //printf("NR:%s\n",str);
                                        i = 0;
                                        while(i < tmp2 && c != '\n'){
                                                c = str[i];
                                                ++i;
                                        }
                                        if(tmp2 < N) break;
                                }
				if(tmp2 < N) break;
                                left = STRLEN;
                                new_str = str+i;
				//printf("LEFT:%s\n",new_str);
                                j = i;
                                while(i < tmp2){
                                        c = str[i];
                                        --left;
                                        ++i;
                                        if(c == '\n') left = STRLEN;
                                }
                                        //str[tmp2] = '\0';
                                write(fd_out,NEWLINE,1);
                                write(fd_out,new_str,tmp2-j);
                                        //printf("'%s\n",new_str);
                                        //left = 50 - tmp2+i;
                                continue;
                        }
                        else{
                                write(fd_out,NEWLINE,1);
                                //left = 50 - tmp + i + 1;
                                //printf("%s\n",str+i+1);
				write(fd_out,str+i+1,tmp-i-1);
				left = STRLEN;
				++i;
				while(i < tmp){
					c = str[i];
					++i;
					--left;
					//printf("% d %d %c",i,tmp,c);
					if(c == '\n') left = STRLEN;
				}
                                continue;
                        }
                }
                        //printf("REG:%s\n",str);
                        //if(c == EOF){printf("EOF:"); write(fd_out,str,i-1);}
                else write(fd_out,str,i);
                        //if(c == EOF/* || tmp < N*/) break;
        }
        //if(tmp < N/* || c == EOF*/) break;
        close(fd_in);
        close(fd_out);
        return 0;
}

