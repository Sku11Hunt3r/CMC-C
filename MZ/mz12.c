#include<stdio.h>
#include<stdlib.h>
struct tnode{
	int key;
	struct tnode *left,*right;
};
struct tnode* mktree(struct tnode* p,int nkey){
	if(p == NULL){
		p = (struct tnode*)malloc(sizeof(struct tnode));
		if(p == NULL) return NULL;
		p->left = p->right = NULL;
		p-> key = nkey;
		return p;
	}
	else if(p -> key > nkey) p->left =  mktree(p->left, nkey);
	else if(p -> key < nkey) p->right =  mktree(p->right, nkey);
	return p;
}
void printt(struct tnode* p){
	if(p != NULL){
		if(p->left != NULL){printf("L"); printt(p->left);}
		printf("%d ",p->key);
		if(p->right != NULL){printf("R"); printt(p->right);}
	}
	return;
}
void print_2min(struct tnode* p){
	if(p->left == NULL){
		printf("%d ",p->key);
		p = p->right;
		while(p->left != NULL) p = p->left;
		printf("%d", p->key);
		return;
	}
	while(p->left->left != NULL) p = p->left;
	printf("%d ",p->left->key);
	if(p->left->right != NULL){
		p = p->left->right;
		while(p->left != NULL) p = p->left;
		printf("%d",p->key);
	}
	else printf("%d", p->key);
}
void del_tree(struct tnode* p){
	if(p == NULL) return;
	if(p->left != NULL) del_tree(p->left);
	if(p->right != NULL) del_tree(p->right);
	free(p);
	p = NULL;
	return;
}
int main(){
	int x;
	struct tnode* tree = NULL;
	while(scanf("%d",&x) == 1) tree = mktree(tree,x);
	if(tree == NULL) return 0;
	//printt(tree);
	print_2min(tree);
	del_tree(tree);
	return 0;
}
