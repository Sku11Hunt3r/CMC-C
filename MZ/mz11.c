#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct node{
	char obj[21];
	struct node *next;
	struct node *prev;
};
struct node* add_end(struct node *head, char *buf, struct node **q){
	struct node *p = (struct node*)malloc(sizeof(struct node));
        if(p == NULL){
		fprintf(stderr,"Not enough memory");
		return head;
	}
	strcpy(p->obj,buf);
        if(head == NULL){
		head = *q = p;
                p->prev = NULL;
        }
        else{
		p->prev = *q;
        	(*q)->next = p;
        	*q = (*q)->next;
        }
	p->next = NULL;
	return head;
}
struct node* delete(struct node *head, char *str){
	if(head == NULL) return NULL;
	struct node *p = head;
	while(strcmp(p->obj,str) != 0){
		if(p->next == NULL) return head;
		p = p->next;
	}
	if(p != head) p->prev->next = p->next;
	else head = head->next;
	if(p->next != NULL) p->next->prev = p->prev;
	p->next = p->prev = NULL;
	free(p);
	return head;
}
struct node* del_node(struct node* head){
        while(head->next != NULL){
                struct node* p = head;
                head = head -> next;
                head -> prev = NULL;
                p->prev = p->next = NULL;
		free(p);
	}
	struct node* p = head;
        head = NULL;
        p->prev = p->next = NULL;
        free(p);
	return NULL;
}
int main(){
	char str[21], buf[21];
	int c,k = 0,i;
	struct node *head = NULL,*p,*q; 
	c = getchar();
	while(c == ' ') c = getchar();
	while(c != '\n' && c != ' '){
		str[k] = c;
		++k;
		c = getchar();
	}
	while(c != '\n') c = getchar(); 
	str[k] = '\0';
	k = 0;
	c = getchar();
	buf[0] = ' ';
	for(i = 1; i < 21; ++i) buf[i] = '\0';
	while(c != '\n' && c != EOF){
		if(c != ' '){
		       buf[k] = c;
		       ++k;
		}
		else{
			if(buf[0] != ' '){
				int i;
				head = add_end(head, buf, &q);
				buf[0] = ' ';
				for(i = 1; i < 20; ++i) buf[i] = '\0';
				k = 0;
			}
		}
		c = getchar();
	}
	if(buf[0] != ' ') head = add_end(head, buf, &q);
	if(head == NULL){
		printf("empty\n");
		return 1;
	}
	head = delete(head,str);
	if(head == NULL){
		printf("empty");
		return 0;
	}
	p = head;
	while(p != NULL){
		if(p->next == NULL) printf("%s",p->obj);
		else printf("%s ",p->obj);
		++i;
		p = p->next;
	}
	head = del_node(head);
	return 0;
}
