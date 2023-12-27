#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
int wordlen(int fd){
	long cur;
	int k = 0;
	char c;
	cur = lseek(fd,(-1)*sizeof(char),SEEK_CUR);
	//printf("CUR%ld\n",cur);
	while(read(fd,&c,sizeof(char)) == sizeof(char)){
		if(c == ' ' || c == ',' || c == '\n' || c =='.') break;
		//printf("C:%d %d K:%d\n",c,d,k);
		++k;
	}
	lseek(fd,cur,SEEK_SET);
	//printf("C+K%ld %d\n",cur,k);
	return k;
}
int str2int(char*str){
       	int k = 0,sign = 1;
	while(*str != '\0'){
		if(*str =='-'){
			sign = -1;
			++str;
			continue;
		}
		if(*str != '-' && *str <='0' && *str >='9') return -1;
		k = k*10 + (*str-'0');
		++str;
	}
	return k*sign;
}	
void add_head(int fd,int add,int head_tail){
	long end,cur,i;
	char c,buf;
	int len;
	lseek(fd,0,SEEK_SET);
	while(read(fd,&c,sizeof(char)) == sizeof(char)){
		//printf("C1:%c\n",c);
		if(c != ' ' && c != ',' && c != '.' && c != '\n'){
			if((len=wordlen(fd)) >= head_tail){
				//printf("%c LEN:%d\n",c,len);
				cur = lseek(fd,0,SEEK_CUR);		
				end = lseek(fd,-1,SEEK_END);
				//printf("C+E%ld %ld\n",cur,end);
				for(i = add; i >=(cur - end + add);--i){
					lseek(fd,(end+i-add)*sizeof(char),SEEK_SET);
				        read(fd,&buf,sizeof(char));
        				lseek(fd,(end+i)*sizeof(char),SEEK_SET);
       					write(fd,&buf,sizeof(char));
					//printf("%c\n",buf);	
				}
				lseek(fd,(cur+add+len)*sizeof(char),SEEK_SET);
			}
			else{
				lseek(fd,len*sizeof(char),SEEK_CUR);
			}
		}
	}
}
void add_tail(int fd,int add,int head_tail){
	long end,cur,i;
	char c,buf;
        int len;
        lseek(fd,0,SEEK_SET);
        while(read(fd,&c,sizeof(char)) == sizeof(char)){
                if(c != ' ' && c != ',' && c != '.' && c != '\n'){
                        //printf("%c %d\n",c,len);
			if((len=wordlen(fd)) >= head_tail){
                                //printf("%d %c\n",len,c);
				cur = lseek(fd,0,SEEK_CUR);
                                end = lseek(fd,-1,SEEK_END);
                                //printf("%ld %ld %d\n",cur,end,len);
                                for(i = add; i >=(cur - end + len);--i){
                                        lseek(fd,(end+i-add)*sizeof(char),SEEK_SET);
                                        read(fd,&buf,sizeof(char));
                                        lseek(fd,(end+i)*sizeof(char),SEEK_SET);
                                        write(fd,&buf,sizeof(char));
                                        //printf("%c\n",buf);
                                }
                                lseek(fd,(cur+add+len)*sizeof(char),SEEK_SET);
                        }
                        else{
                                lseek(fd,len*sizeof(char),SEEK_CUR);
                        }
                }
        }
}
void add_head_tail(int fd,int head,int tail){
	add_head(fd,head,head+tail);
	add_tail(fd,tail,head+tail);
}
int main(int argc,char** argv){
	int fd,tmp;
	char c;
	fd = open(argv[1],O_RDWR,0644);
	if(fd < 0){
		fprintf(stderr,"Can't open file");
		return 1;
	}
	c = argv[3][0];
	//printf("%c\n",c);
	switch(c){
	case 't':
		tmp = str2int(argv[4]);
		//printf("%d\n",tmp);
		add_tail(fd,tmp,tmp);
		break;
	case 'h':
		tmp = str2int(argv[4]);
		add_head(fd,tmp,tmp);
		break;
	case 'e':
		add_head_tail(fd,str2int(argv[4]),str2int(argv[5]));
		break;
	default:
		printf("ERROR\n");
		break;
	}
	return 0;
}
