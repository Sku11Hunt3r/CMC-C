#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#define N 13
int main(int argc, char** argv){
        if(argc != 3){
                fprintf(stderr,"Error\n");
                return 0;
        }
        if(argv[1] == NULL || argv[2] == NULL){
                fprintf(stderr,"Error\n");
                return 0;
        }
        int fd_in,fd_out,left = 50,tmp;
        char str[N],c,new_line[1] = "\n";
        fd_in = open(argv[1],O_RDONLY);
        fd_out = creat(argv[2],644);
        while(c != EOF){
                while((tmp = read(fd_in,str,N))/* == N*/ && left > 0){
                        //printf("NEWSTR:%s\n",str);
			int i = 0;
                        c = str[i];
                        while(/*c != EOF &&*/ i < tmp && left > 0){
                                c = str[i];
                                --left;
                                ++i;
				if(c == '\n') left = 50;
                        }
                        str[i] = '\0';
			//printf("STRLEFT:%s\n",str);
                        if(left == 0){
                                char* new_str;
                                int tmp2,j;
				write(fd_out,str,i);
				while(str[i] != '\n' && i < tmp) ++i;
				//printf("LEFTAFTI:%s\n",str+i);
				if(i == tmp){
                                	while(c != '\n'){
						tmp2 = read(fd_in,str,N);
                                        	//printf("NR:%s\n",str);
						i = 0;
                                        	while(i < tmp2 && c != '\n'){
                                                	c = str[i];
                                               		++i;
                                        	}
                                	}
					left = 50;
					new_str = str+i;
                                	//printf("LEFT:%s\n",new_str);
					j = i;
					while(i < tmp2){
	                                	c = str[i];
						--left;
	                                	++i;
	                                	if(c == '\n') left = 50;
	                        	}
                                	str[tmp2] = '\0';
                                	write(fd_out,new_line,1);
                                	write(fd_out,new_str,tmp2-j);
                                	//printf("'%s\n",new_str);
					//left = 50 - tmp2+i;
                                	continue;
				}
				else{
					write(fd_out,new_line,1);
					left = 50 -tmp + i;
					write(fd_out,str+i,tmp-i);
					continue;
				}
                        }
			//printf("REG:%s\n",str);
                        if(c == EOF) write(fd_out,str,i-1);
                        else write(fd_out,str,i);
                        if(c == EOF/* || tmp < N*/) break;
                }
                if(tmp < N || c == EOF) break;
        }
        close(fd_in);
        close(fd_out);
        return 0;
}
