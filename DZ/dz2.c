#include <stdio.h>
#include  <stdlib.h>
#define HALFBYTE 4
#define BYTE 8
unsigned char kol_bit(unsigned char a){
    unsigned char k = 0;
    while(a != 0){
        ++k;
        a >>= 1;
    }
    return k;
}
int space(unsigned char* c){
    int amount = c[0],k = 0,i;
    if(amount%2 == 0) amount/=2;
    else amount = amount/2 + 1;
    for(i = 1; i <= c[0]; ++i) k += kol_bit(c[i]);
    if(k%BYTE != 0) k += (BYTE - k%BYTE);
    return (k/8 + amount + 1);
}
unsigned char* packing(unsigned char* c,int kol_byte){
    int n = kol_byte,i,left, amount = c[0];
    unsigned char* p;
    unsigned char* arr = (unsigned char*) calloc (n, sizeof(char));
    if(arr == NULL){
	    fprintf(stderr,"Not enough memory\n");
	    return NULL;
    }
    if(amount%2 == 0) amount/=2;
    else amount = amount/2 + 1;
    arr[0] = c[0];
    for (i = 1; i <= amount; ++i){
        arr[i] = (kol_bit(c[2*i-1])) << HALFBYTE;
        if(2*i <= c[0]) arr[i] |= kol_bit(c[2*i]);
    }
    p = arr + 1 + amount;
    left = BYTE - 1;
    unsigned char mask = 1 << (BYTE - 1);
    for(i = 1;i <= c[0];++i){
        unsigned char tmp = c[i];
        while((tmp&mask) == 0 && (mask > 0)) mask >>= 1;
        if(mask != 0){
            while(mask!=0){
                if((mask&tmp) != 0) *p |= 1 << left;
                if(left == 0){
                    ++p;
                    left = BYTE;
                }
                --left;
                mask>>=1;
            }
        }
        mask = 1<<7;
    }
    return arr;
}
void print_arr(unsigned char* arr,int kol_byte){
    int i,j;
    for(i = 0; i < kol_byte;++i){
        unsigned char tmp = arr[i];
        for(j = 0;j < 8;++j){
            unsigned char tmp2;
            tmp2 = tmp & (1 << (BYTE - 1));
            tmp2 >>= (BYTE - 1);
            printf("%hhd",tmp2);
            tmp <<= 1;
        }
        printf(" ");
    }
}
int main(){
    int i,kol_byte;
    unsigned char x;
    scanf("%hhd",&x);
    unsigned char* c = (unsigned char*) calloc(x+1, sizeof(char));
    if(c == NULL){
	fprintf(stderr,"Not enough memory\n");
	return 1;
    }
    c[0] = x;
    for(i = 1; i <= c[0];++i){
        scanf("%hhd",&x);
        c[i] = x;
    }
    kol_byte = space(c);
    unsigned char* arr = (unsigned char*) calloc (kol_byte, sizeof(char));
    if(arr == NULL){
	    fprintf(stderr,"Not enough memory\n");
	    return 1;
    }
    arr = packing(c,kol_byte);
    print_arr(arr,kol_byte);
    free(arr);
    free(c);
    return 0;
}
