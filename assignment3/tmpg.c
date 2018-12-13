#include <stdio.h>
#include <stdlib.h>
#include "tmpg.h"

#define MAX_COST 99999999

extern struct Person people[100];
//array of st_nodes
extern struct st_node st_arr[100];
//transportation of [src][dest] exists:1, else:0
extern int tp_visit[100][100];
//tp_arr[src site num][dest site num]
extern struct tp_node tp_arr[100][100];

// ! create st_nodes
struct st_node *newStNode(int st_seq)
{
	struct st_node *newNode =
		(struct st_node *)malloc(sizeof(struct st_node));
	newNode->st_seq = st_seq;
	//tour days : 1~5 days
	newNode->tour_time = 1 + rand() % 5;
	newNode->ht_root = NULL;
	return newNode;
}

// !
void createSites()
{
	int i;
	for (i = 0; i < 100; i++)
	{
		struct st_node *newNode = newStNode(i);
		st_arr[i] = *(newNode);
	}
}

//This graph is only for connection relationship among sites.
//The node has the number of site.
//To access the site struct, use st_arr[node->stnum]
struct st_for_graph *newStGraphNode(int st_seq)
{
	struct st_for_graph *newNode = (struct st_for_graph *)malloc(sizeof(struct st_for_graph));
	newNode->stnum = st_seq;
	newNode->next = NULL;
	return newNode;
}

struct Graph *createGraph(int V)
{
	struct Graph *graph =
		(struct Graph *)malloc(sizeof(struct Graph));
	graph->V = V;

	graph->array =
		(struct st_num_list *)malloc(V * sizeof(struct st_num_list));

	// initialize heads as NULL
	int i;
	for (i = 0; i < V; ++i)
		graph->array[i].head = NULL;

	return graph;
}

// adds an edge from src to dest
void addEdge(struct Graph *graph, int src, int dest)
{
	struct st_for_graph *tmp = newStGraphNode(dest);
	tmp->next = graph->array[src].head;
	graph->array[src].head = tmp;
}

void printGraph(struct Graph *graph)
{
	int seq, v;
	for (v = 0; v < graph->V; ++v)
	{
		struct st_for_graph *pCrawl = graph->array[v].head;
		printf("\n #%d ", v);
		while (pCrawl)
		{
			seq = pCrawl->stnum;
			printf("-> %d(days:%d)", pCrawl->stnum, st_arr[seq].tour_time);
			pCrawl = pCrawl->next;
		}
	}
}

//Hotel rbtree and Reservation rbtree are almost same
//Hotel red black tree
htnode ht_initilize(htnode p, int v)
{
	htnode tree = (htnode)malloc(sizeof(struct ht_node));
	tree->left = tree->right = NULL;
	tree->parent = p;
	tree->ht_seq = v;
	tree->color = RED;

	//hotel price : 30000~70000
	tree->ht_price = 30000 + (rand() % 700) * 100;
	return tree;
}
htnode ht_grandparent(htnode n)
{
	if (n == NULL || n->parent == NULL)
		return NULL;
	return n->parent->parent;
}
htnode ht_uncle(htnode n)
{
	htnode g = ht_grandparent(n);
	if (n == NULL || g == NULL)
		return NULL;
	if (n->parent == g->left)
		return g->right;
	else
		return g->left;
}
htnode ht_sibling(htnode n)
{
	if (n == n->parent->left)
		return n->parent->right;
	else
		return n->parent->left;
}
int ht_colorOf(htnode n)
{
	return n == NULL ? BLACK : n->color;
}
void ht_insertNode(httree t, int v)
{
	int pl = 0;
	htnode ptr, btr = NULL, newNode;

	for (ptr = *t; ptr != NULL;
		 btr = ptr, ptr = ((pl = (ptr->ht_seq > v)) ? ptr->left : ptr->right))
		;

	newNode = ht_initilize(btr, v);

	if (btr != NULL)
		(pl) ? (btr->left = newNode) : (btr->right = newNode);

	ht_insertUtil(newNode);

	ptr = newNode;
	for (ptr = newNode; ptr != NULL; btr = ptr, ptr = ptr->parent)
		;
	*t = btr;
}
void ht_insertUtil(htnode n)
{
	htnode u = ht_uncle(n), g = ht_grandparent(n), p = n->parent;
	if (p == NULL)
		n->color = BLACK;
	else if (p->color == BLACK)
		return;
	else if (u != NULL && u->color == RED)
	{
		p->color = BLACK;
		u->color = BLACK;
		g->color = RED;

		ht_insertUtil(g);
	}
	else
	{
		if (n == p->right && p == g->left)
		{
			ht_rotateLeft(p);
			n = n->left;
		}
		else if (n == p->left && p == g->right)
		{
			ht_rotateRight(p);
			n = n->right;
		}

		g = ht_grandparent(n);
		p = n->parent;

		p->color = BLACK;
		g->color = RED;

		if (n == p->left)
			ht_rotateRight(g);
		else
			ht_rotateLeft(g);
	}
}
void ht_replaceNode(httree t, htnode o, htnode n)
{
	if (o->parent == NULL)
		*t = n;
	else
	{
		if (o == o->parent->left)
			o->parent->left = n;
		else
			o->parent->right = n;
	}

	if (n != NULL)
		n->parent = o->parent;
}
void ht_removeNode(httree t, int v)
{
	htnode n = ht_findNode(t, v), c;

	if (n == NULL)
		return;
	if (n->left != NULL && n->right != NULL)
	{
		htnode pred = n->left;
		while (pred->right != NULL)
			pred = pred->right;
		n->ht_seq = pred->ht_seq;
		n = pred;
	}

	c = n->right == NULL ? n->left : n->right;
	if (n->color == BLACK)
	{
		n->color = ht_colorOf(c);
		ht_removeUtil(n);
	}

	ht_replaceNode(t, n, c);
	free(n);
}
void ht_removeUtil(htnode n)
{
	if (n->parent == NULL)
		return;

	htnode s = ht_sibling(n);
	if (ht_colorOf(s) == RED)
	{
		n->parent->color = RED;
		s->color = BLACK;
		if (n == n->parent->left)
			ht_rotateLeft(n->parent);
		else
			ht_rotateRight(n->parent);
	}

	s = ht_sibling(n);
	if (ht_colorOf(n->parent) == BLACK && ht_colorOf(s) == BLACK &&
		ht_colorOf(s->left) == BLACK && ht_colorOf(s->right) == BLACK)
	{
		s->color = RED;
		ht_removeUtil(n->parent);
	}
	else if (ht_colorOf(n->parent) == RED && ht_colorOf(s) == BLACK &&
			 ht_colorOf(s->left) == BLACK && ht_colorOf(s->right) == BLACK)
	{
		s->color = RED;
		n->parent->color = BLACK;
	}
	else
	{
		if (n == n->parent->left && ht_colorOf(s) == BLACK &&
			ht_colorOf(s->left) == RED && ht_colorOf(s->right) == BLACK)
		{
			s->color = RED;
			s->left->color = BLACK;
			ht_rotateRight(s);
		}
		else if (n == n->parent->right && ht_colorOf(s) == BLACK &&
				 ht_colorOf(s->right) == RED && ht_colorOf(s->left) == BLACK)
		{
			s->color = RED;
			s->right->color = BLACK;
			ht_rotateLeft(s);
		}

		s->color = ht_colorOf(n->parent);
		n->parent->color = BLACK;
		if (n == n->parent->left)
		{
			s->right->color = BLACK;
			ht_rotateLeft(n->parent);
		}
		else
		{
			s->left->color = BLACK;
			ht_rotateRight(n->parent);
		}
	}
}

void ht_rotateRight(htnode tree)
{
	htnode c = tree->left;
	htnode p = tree->parent;

	if (c->right != NULL)
		c->right->parent = tree;

	tree->left = c->right;
	tree->parent = c;
	c->right = tree;
	c->parent = p;

	if (p != NULL)
	{
		if (p->right == tree)
			p->right = c;
		else
			p->left = c;
	}
}
void ht_rotateLeft(htnode tree)
{
	htnode c = tree->right;
	htnode p = tree->parent;

	if (c->left != NULL)
		c->left->parent = tree;

	tree->right = c->left;
	tree->parent = c;
	c->left = tree;
	c->parent = p;

	if (p != NULL)
	{
		if (p->left == tree)
			p->left = c;
		else
			p->right = c;
	}
}

htnode ht_findNode(httree t, int v)
{
	htnode p;
	for (p = *t; p != NULL && p->ht_seq != v; p = (p->ht_seq > v ? p->left : p->right))
		;
	return p;
}
void ht_printNode(htnode n)
{
	printf("%d(%s)(%dwon) ", n->ht_seq, (n->color == BLACK ? "b" : "r"), n->ht_price);
}
void ht_inorderIterator(htnode n, void (*func)(htnode))
{
	if (n == NULL)
		return;
	ht_inorderIterator(n->left, func);
	func(n);
	ht_inorderIterator(n->right, func);
}
void ht_inorderPrint(httree t)
{
	ht_inorderIterator(*t, ht_printNode);
	printf("\n");
}
void ht_destroy(htnode tree)
{
	if (tree == NULL)
		return;
	ht_destroy(tree->left);
	ht_destroy(tree->right);
	free(tree);
}
//End of hotel red black tree

//----------------------------------------

//Reservation red black tree
rvnode rv_initilize(rvnode p, int v, int start, int end, int price)
{
	rvnode tree = (rvnode)malloc(sizeof(struct reserv_node));
	tree->left = tree->right = NULL;
	tree->parent = p;
	tree->customer_id = v;
	tree->tour_start = start;
	tree->tour_end = end;
	tree->price = price;
	tree->color = RED;

	return tree;
}
rvnode rv_grandparent(rvnode n)
{
	if (n == NULL || n->parent == NULL)
		return NULL;
	return n->parent->parent;
}
rvnode rv_uncle(rvnode n)
{
	rvnode g = rv_grandparent(n);
	if (n == NULL || g == NULL)
		return NULL;
	if (n->parent == g->left)
		return g->right;
	else
		return g->left;
}
rvnode rv_sibling(rvnode n)
{
	if (n == n->parent->left)
		return n->parent->right;
	else
		return n->parent->left;
}
int rv_colorOf(rvnode n)
{
	return n == NULL ? BLACK : n->color;
}
void rv_insertNode(rvtree t, int v, int start, int end, int price)
{
	int pl = 0;
	rvnode ptr, btr = NULL, newNode;

	for (ptr = *t; ptr != NULL;
		 btr = ptr, ptr = ((pl = (ptr->customer_id > v)) ? ptr->left : ptr->right))
		;

	newNode = rv_initilize(btr, v, start, end, price);

	if (btr != NULL)
		(pl) ? (btr->left = newNode) : (btr->right = newNode);

	rv_insertUtil(newNode);

	ptr = newNode;
	for (ptr = newNode; ptr != NULL; btr = ptr, ptr = ptr->parent)
		;
	*t = btr;
}
void rv_insertUtil(rvnode n)
{
	rvnode u = rv_uncle(n), g = rv_grandparent(n), p = n->parent;
	if (p == NULL)
		n->color = BLACK;
	else if (p->color == BLACK)
		return;
	else if (u != NULL && u->color == RED)
	{
		p->color = BLACK;
		u->color = BLACK;
		g->color = RED;

		rv_insertUtil(g);
	}
	else
	{
		if (n == p->right && p == g->left)
		{
			rv_rotateLeft(p);
			n = n->left;
		}
		else if (n == p->left && p == g->right)
		{
			rv_rotateRight(p);
			n = n->right;
		}

		g = rv_grandparent(n);
		p = n->parent;

		p->color = BLACK;
		g->color = RED;

		if (n == p->left)
			rv_rotateRight(g);
		else
			rv_rotateLeft(g);
	}
}
void rv_replaceNode(rvtree t, rvnode o, rvnode n)
{
	if (o->parent == NULL)
		*t = n;
	else
	{
		if (o == o->parent->left)
			o->parent->left = n;
		else
			o->parent->right = n;
	}

	if (n != NULL)
		n->parent = o->parent;
}
void rv_removeNode(rvtree t, int v)
{
	rvnode n = rv_findNode(t, v), c;

	if (n == NULL)
		return;
	if (n->left != NULL && n->right != NULL)
	{
		rvnode pred = n->left;
		while (pred->right != NULL)
			pred = pred->right;
		n->customer_id = pred->customer_id;
		n = pred;
	}

	c = n->right == NULL ? n->left : n->right;
	if (n->color == BLACK)
	{
		n->color = rv_colorOf(c);
		rv_removeUtil(n);
	}

	rv_replaceNode(t, n, c);
	free(n);
}
void rv_removeUtil(rvnode n)
{
	if (n->parent == NULL)
		return;

	rvnode s = rv_sibling(n);
	if (rv_colorOf(s) == RED)
	{
		n->parent->color = RED;
		s->color = BLACK;
		if (n == n->parent->left)
			rv_rotateLeft(n->parent);
		else
			rv_rotateRight(n->parent);
	}

	s = rv_sibling(n);
	if (rv_colorOf(n->parent) == BLACK && rv_colorOf(s) == BLACK &&
		rv_colorOf(s->left) == BLACK && rv_colorOf(s->right) == BLACK)
	{
		s->color = RED;
		rv_removeUtil(n->parent);
	}
	else if (rv_colorOf(n->parent) == RED && rv_colorOf(s) == BLACK &&
			 rv_colorOf(s->left) == BLACK && rv_colorOf(s->right) == BLACK)
	{
		s->color = RED;
		n->parent->color = BLACK;
	}
	else
	{
		if (n == n->parent->left && rv_colorOf(s) == BLACK &&
			rv_colorOf(s->left) == RED && rv_colorOf(s->right) == BLACK)
		{
			s->color = RED;
			s->left->color = BLACK;
			rv_rotateRight(s);
		}
		else if (n == n->parent->right && rv_colorOf(s) == BLACK &&
				 rv_colorOf(s->right) == RED && rv_colorOf(s->left) == BLACK)
		{
			s->color = RED;
			s->right->color = BLACK;
			rv_rotateLeft(s);
		}

		s->color = rv_colorOf(n->parent);
		n->parent->color = BLACK;
		if (n == n->parent->left)
		{
			s->right->color = BLACK;
			rv_rotateLeft(n->parent);
		}
		else
		{
			s->left->color = BLACK;
			rv_rotateRight(n->parent);
		}
	}
}

void rv_rotateRight(rvnode tree)
{
	rvnode c = tree->left;
	rvnode p = tree->parent;

	if (c->right != NULL)
		c->right->parent = tree;

	tree->left = c->right;
	tree->parent = c;
	c->right = tree;
	c->parent = p;

	if (p != NULL)
	{
		if (p->right == tree)
			p->right = c;
		else
			p->left = c;
	}
}
void rv_rotateLeft(rvnode tree)
{
	rvnode c = tree->right;
	rvnode p = tree->parent;

	if (c->left != NULL)
		c->left->parent = tree;

	tree->right = c->left;
	tree->parent = c;
	c->left = tree;
	c->parent = p;

	if (p != NULL)
	{
		if (p->left == tree)
			p->left = c;
		else
			p->right = c;
	}
}
rvnode rv_findNode(rvtree t, int v)
{
	rvnode p;
	for (p = *t; p != NULL && p->customer_id != v; p = (p->customer_id > v ? p->left : p->right))
		;
	return p;
}
void rv_printNode(rvnode n)
{
	printf("%d(%s) ", n->customer_id, (n->color == BLACK ? "b" : "r"));
}
void rv_inorderIterator(rvnode n, void (*func)(rvnode))
{
	if (n == NULL)
		return;
	rv_inorderIterator(n->left, func);
	func(n);
	rv_inorderIterator(n->right, func);
}
void rv_inorderPrint(rvtree t)
{
	rv_inorderIterator(*t, rv_printNode);
	printf("\n");
}
void rv_destroy(rvnode tree)
{
	if (tree == NULL)
		return;
	rv_destroy(tree->left);
	rv_destroy(tree->right);
	free(tree);
}
//end of reservation red black tree

//Dijkstra's algorithm(find cheapest path)
int dijkstra_cost(struct Graph *graph, int origin, int destination)
{
	int min, i, j, cur, m, c;
	int p = 0;
	int selected[100];
	int cost[100][100];
	int sum[100];
	int prev[100];
	int path[100];

	for (i = 0; i < 100; i++)
	{
		selected[i] = 0;
		prev[i] = -1;
		sum[i] = MAX_COST;
	}

	for (i = 0; i < 100; i++)
	{
		for (j = 0; j < 100; j++)
		{
			if (tp_arr[i][j].tp_price != 0)
				cost[i][j] = tp_arr[i][j].tp_price;
			else
				cost[i][j] = MAX_COST;
		}
	}

	sum[origin] = 0;
	selected[origin] = 1;
	cur = origin;
	int check = 0;

	while (selected[destination] == 0)
	{
		min = MAX_COST;
		m = 0;

		for (i = 1; i < 100; i++)
		{
			c = sum[cur] + cost[cur][i];
			if (c < sum[i] && selected[i] == 0)
			{
				sum[i] = c;
				prev[i] = cur;
			}
			if (min > sum[i] && selected[i] == 0)
			{
				min = sum[i];
				m = i;
			}
		}
		cur = m;
		selected[cur] = 1;
		check++;
		if (check > 1000)
			break;
	}
	if (check > 1000)
		return 1;

	cur = destination;
	j = 0;
	while (cur != -1)
	{
		path[j++] = cur;
		cur = prev[cur];
	}
	printf("The cheapest route is site[%d]", path[--j]);
	for (i = j - 1; i >= 0; i--)
	{
		printf("->site[%d]", path[i]);
	}
	printf("\n");

	return sum[destination];
}
//Dijkstra's algorithm(find shortest path)
int dijkstra_distance(struct Graph *graph, int origin, int destination)
{
	int min, i, j, cur, m, c;
	int p = 0;
	int selected[100]; //nodes passed
	int dist[100][100];
	int sum[100];
	int prev[100];
	int path[100];

	for (i = 0; i < 100; i++)
	{
		selected[i] = 0;
		prev[i] = -1;
		sum[i] = MAX_COST;
	}

	for (i = 0; i < 100; i++)
	{
		for (j = 0; j < 100; j++)
		{
			if (tp_arr[i][j].distance != 0)
				dist[i][j] = tp_arr[i][j].distance;
			else
				dist[i][j] = MAX_COST;
		}
	}

	sum[origin] = 0;
	selected[origin] = 1;
	cur = origin;
	int check = 0;

	while (selected[destination] == 0)
	{
		min = MAX_COST;
		m = 0;

		for (i = 1; i < 100; i++)
		{
			c = sum[cur] + dist[cur][i];
			if (c < sum[i] && selected[i] == 0)
			{
				sum[i] = c;
				prev[i] = cur;
			}
			if (min > sum[i] && selected[i] == 0)
			{
				min = sum[i];
				m = i;
			}
		}
		cur = m;
		selected[cur] = 1;
		check++;
		if (check > 1000)
			break;
	}
	if (check > 1000)
		return 1;

	cur = destination;
	j = 0;
	while (cur != -1)
	{
		path[j++] = cur;
		cur = prev[cur];
	}
	printf("The shortest route is site[%d]", path[--j]);
	for (i = j - 1; i >= 0; i--)
	{
		printf("->site[%d]", path[i]);
	}
	printf("\n");

	return sum[destination];
}
