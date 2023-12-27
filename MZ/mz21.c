#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<limits.h>
int main(int argc, char** argv){
    if(argc != 3) return 0;
    if(argv[1] == NULL ||  argv[2] == NULL) return 0;
    int i,size = 0,fd_in, fd_out, tmp,a1,a2,min,minpos;
    fd_in = open(argv[1],O_RDWR,0644);
    fd_out = creat(argv[2],0644);
    if(fd_in == -1 ||  fd_out == -1){
	    if(fd_in > 0) close(fd_in);
	    return 0;
    }
    while((tmp = read(fd_in,&a1,sizeof(int)))){
    	++size;
        if(tmp < sizeof(int)) return 0;
    }
    for(i = size;i > 0;--i){
        int k = 0;
        min = INT_MAX;
        lseek(fd_in,0,SEEK_SET);
        while(read(fd_in,&a1,sizeof(int)) && k < i){
            k++;
            if(a1 < min){
                min = a1;
                minpos = lseek(fd_in,0,SEEK_CUR) - sizeof(int);
            }
        }
        write(fd_out,&min,sizeof(int));
        lseek(fd_in,(i-1)*sizeof(int),SEEK_SET);
        read(fd_in,&a2,sizeof(int));
        lseek(fd_in,(i-1)*sizeof(int),SEEK_SET);
        write(fd_in,&min,sizeof(int));
        lseek(fd_in,minpos,SEEK_SET);
        write(fd_in,&a2,sizeof(int));
    }
    close(fd_in);
    close(fd_out);
    return 0;
}
