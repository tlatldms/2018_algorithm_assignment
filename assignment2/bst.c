#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Node {
	struct Node* parent;
	struct Node* left;
	struct Node* right;
	int key;
};

void print_tree(struct Node* node) {
	if (node == NULL ) return;
	printf("%d ", node->key);
	print_tree(node->left);
	print_tree(node->right);
}

void insert(struct Node* root, int key) {
	struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
	new_node->parent = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->key = key;
	struct Node* cur = root;

	while (1) {
		if (root->key == -1) {
			root->key = key;
			return;
		}
		if (key == cur->key) return;
		else if (key > cur->key) {
			if (cur->right == NULL) {
				cur->right = new_node;
				new_node->parent = cur;
				return;
			}
			else cur = cur->right;
		}
		else {
			if (cur->left == NULL) {
				cur->left = new_node;
				new_node->parent = cur;
				return;
			}
			else cur = cur->left;
		}
	}
}

int nearest_nb(struct Node* root, int key) {
	int min=99999, min_key=-1;
	while (1) {
		if (root == NULL) break;
		if (root->key == key) return key;
		if (min > abs(root->key - key)) {
			min = abs(root->key - key);
			min_key = root->key;
		}
		if (key < root->key) root = root->left;
		else root = root->right;
	}
	return min_key;
}

void delete_tree(struct Node** _root, int key) {
	struct Node* root = *_root;
	struct Node* cur = root, *parent= NULL;
	while (cur!= NULL && cur->key!=key) {
		parent =cur;
		if (cur->key > key)
			cur= cur->left;
		else cur = cur->right;
	}
	if (cur == NULL) {
		printf("There is no %d. Cannot delete it\n", key);
		return;
	}
	//no child
	if (cur->left == NULL && cur->right == NULL) {
		if (parent != NULL) {
			if (parent->left == cur)
				parent->left == NULL;
			else parent->right == NULL;
		}
		else root == NULL;
	}
	// one child
	else if (cur->left == NULL || cur->right == NULL) {
		struct Node* child;
		if (cur->left != NULL)
			child = cur->left;
		else child = cur->right;

		if (parent != NULL) {
			if (parent->left == cur){
				parent->left = child;
				child->parent = parent;
			}
			else {
				parent->right = child;
				child->parent = parent;
			}
		}
		else *_root = child;
	}
	//two child
	else {
		struct Node* succ_parent = cur, *succ = cur->right;
		while (succ->left != NULL) {
			succ_parent = succ;
			succ = succ->left;
		}

		if (succ_parent->right == succ)
			succ_parent->right = succ->right;
		else succ_parent->left = succ->right;
		cur->key = succ->key;
		cur = succ;
	}
	free(cur);
}

void search_tree(struct Node *root, int key) {
	struct Node* cur = root;
	printf("\nsearch %d\n",key);
	while (1) {
		if (cur == NULL) {
			printf("Not found!");
			break;
		}
		printf("%d  ", cur->key);
		if (cur->key == key) break;
		else if (cur->key > key)
			cur = cur->left;
		else cur=cur->right;
	}
	printf("\n");
}

int main() {
	srand(time(NULL));
	int tmp, i=0;
	int A[20];
	int visit[50] = {0,};
	while (i<20) {
		tmp = rand()%50;
		if (visit[tmp]!=1) {
			visit[tmp]=1;
			A[i++]=tmp;
			
		}
	}

	struct Node* root = malloc(sizeof(struct Node));
	root->parent=NULL;
	root->left=NULL;
	root->right=NULL;
	root->key = -1;
	
	//for (i=5; i<10; i++ )
	for (i=0; i<20; i++ )
		insert(root, A[i]);
	print_tree(root);
	printf("\n");
	search_tree(root,15);
	search_tree(root,8);
	search_tree(root, 18);
	printf("\nnearest 17 : %d\nnearest 3 : %d\nnearest 8 : %d\n",nearest_nb(root,17),nearest_nb(root,3),nearest_nb(root,8));	

	printf("Delete 17\n");
	delete_tree(&root, 17);
	print_tree(root);
	printf("\n\n");
	printf("Delete 5\n");
	delete_tree(&root, 5);
	print_tree(root);
	printf("\n\n");
	printf("Delete 51\n");
	delete_tree(&root, 51);
	print_tree(root);
	printf("\n\n");
	printf("Delete 17\n");
	delete_tree(&root,17);
	print_tree(root);
	printf("\n");

	return 0;
}


