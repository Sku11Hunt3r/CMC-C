#include<stdio.h>
enum flag_in {IN = 1, OUT = 0};
enum flag_in flag;
int main(int argc, char**argv){
	FILE* fo,*fc;
	int symb=0,word=0,str=0,c;
	if(argv[1] == NULL || argv[2] == NULL){
		return 1;
	}
	if(argc != 3) return 1;
	fo = fopen(argv[1],"r");
	if(fo == NULL) return 1;
	c = fgetc(fo);
	while(c != EOF){
		flag = OUT;
		while(c != '\n'){
			if(c == ' '){
				++symb;
				if(flag) ++word;
				flag = OUT;
			}
			else{
				flag = IN;
				++symb;
			}
			c = fgetc(fo);
		}
		if(flag) ++word;
		++symb;
		++str;
		c = fgetc(fo);
	}
	fclose(fo);
	fc = fopen(argv[2],"w");
	if(fc == NULL) return 1;
	fprintf(fc,"%d %d %d",str,word,symb);
	fclose(fc);
	return 0;
}
