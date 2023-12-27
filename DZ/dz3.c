#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#define N 10
#define M 5
int my_strlen(char* str){
	if(str == NULL) return -1;
	char*p = str;
	int k = 0;
        while(*p != '\0' && *p != EOF){
                ++k;
                ++p;
        }
        return k;
}
int my_strcmp(char* first, char* second){
	char *p = first,*q = second;
        if(first == NULL || second == NULL) return INT_MAX;
        while(*p == *q && *p != '\0' && *q != '\0'){
                ++p;
                ++q;
        }
        if(*p == '\0' || *q == '\0'){
                if(*p == '\0' && *q == '\0') return 0;
                else{
                        if(*p > *q) return -1;
                        else return 1;
                }
        }
        else{
                if(*p > *q) return -1;
                else return 1;
        }
}
char* my_strstr(char* src, char* obj){
        char *p = src, *q = obj;
        if(p == NULL || q == NULL) return NULL;
        while(*p != '\0'){
                if(*p == *q){
			char* r = p;
			while(*r == *q && *q != '\0' && *r != '\0'){
				++r;
				++q;
			}
			if(*q == '\0') return p; 
			q = obj;
                }
                ++p;
        }
        return NULL;
}
void print_str_arr(char**arr,int m){
        int j;
        for(j = 0;j < m;++j) printf("%s\n",arr[j]);
}
void search_arr(char** arr, char* obj, int k){
        int i=0,kol;
	while(arr[i] != NULL){
		char* p = arr[i],*q;
                kol = 0;
                while(*p != '\0'){
                        q = my_strstr(p,obj);
			if(q != NULL){
                                ++kol;
                                q += my_strlen(obj);
                                p = q;
                        }
			else break;
                }
                if(kol == k) printf("%d:%s\n",(i+1),arr[i]);
		++i;
	}
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
int main(int argc, char ** argv){
        char **arr = (char**)malloc(M*sizeof(char*));
	if(arr == NULL){
		printf("NOT ENOUGH MEMORY\n");
		return 1;
	}
        int c,m = 0,n = 0,koln = 2, kolm = 2,i;
        c = getchar();
        while(c != EOF){
		n = 0;
                char *str = (char*)malloc(N*sizeof(char));
                if(str == NULL){
			printf("NOT ENOUGH MEMORY\n");
			return 1;
		}
		while(c != '\n' && c != EOF){
                        str[n + (koln - 2)*N] = c;
                        ++n;
                        c = getchar();
                        if(n == N - 1){
				str = (char*)realloc(str,koln*N*sizeof(char));
                                if(str == NULL){
					printf("NOT ENOUGH MEMORY\n");
 					return 1;
				}
				++koln;
                                n = -1;
                        }
		}
                str[(koln-2)*N + n] = '\0';
		koln = 2;
                arr[m + (kolm - 2)*M] = str;
                if(m == M - 1){
                        arr = (char**)realloc(arr,kolm*M*sizeof(char*));
                        if(arr == NULL){
				printf("NOT ENOUGH MEMORY\n");
				return 1;
			}
                        ++kolm;
                        m = -1;
                }
		++m;
                c = getchar();
        }
	arr[m + (kolm-2)*M] = NULL;
        //print_str_arr(arr, m+(kolm-2)*M);
	//for(i = 0;i < (m +(kolm-2)*M);++i) printf("%s %s\n",arr[i], argv[1]);
	if(argv[1] != NULL && argv[2] != NULL && str2int(argv[2]) > 0) search_arr(arr, argv[1], str2int(argv[2]));
	//for(i = 0;i < (m +(kolm-2)*M);++i) printf("%s %s\n",arr[i], argv[1]);
	for(i = 0;i < (m +(kolm-2)*M);++i) free(arr[i]);
	free(arr);
	return 0;
}
