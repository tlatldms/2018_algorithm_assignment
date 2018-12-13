#include <stdlib.h>

enum
{
	RED,
	BLACK
};
typedef int COLOR;

struct tp_node
{
	int tp_seq;
	int tp_price;
	int distance;
} * tpnode;

//site(city) node (Graph)
struct st_node
{
	int st_seq;
	int tour_time;
	struct ht_node *ht_root;
};

struct st_for_graph
{
	int stnum;
	struct st_for_graph *next;
};

//hotel node (RBTree)
typedef struct ht_node
{
	int ht_seq;
	int ht_price;
	COLOR color;
	struct ht_node *left, *right, *parent;
} * htnode;

//reservation node (RBTree)
typedef struct reserv_node
{
	int customer_id;
	int tour_start;
	int tour_end;
	int price;
	COLOR color;
	struct reserv_node *left, *right, *parent;
} * rvnode;

//Root Node of hotel RBTree
typedef htnode *httree;
//Root Node of reservation RBTree
typedef rvnode *rvtree;

//Graph (for sites)
// A structure to represent an adjacency list
struct st_num_list
{
	struct st_for_graph *head;
};

// A structure to represent a graph. A graph
// is an array of adjacency lists.
// Size of array will be V (number of vertices
// in graph)
struct Graph
{
	int V;
	struct st_num_list *array;
};

struct Person
{
	int id;
	int org;
	int dst;
	int budget;
	int start;
	int end;
};

//Hotel red black tree functions
htnode ht_initilize(htnode, int);
htnode ht_grandparent(htnode);
htnode ht_uncle(htnode);
htnode ht_sibling(htnode);
htnode ht_findNode(httree, int);
void ht_insertNode(httree, int);
void ht_insertUtil(htnode);
void ht_removeNode(httree, int);
void ht_removeUtil(htnode);
void ht_rotateRight(htnode);
void ht_rotateLeft(htnode);
htnode ht_findNode(httree, int);
void ht_replaceNode(httree, htnode, htnode);
void ht_printNode(htnode n);
void ht_inorderIterator(htnode n, void (*func)(htnode));
void ht_inorderPrint(httree t);
void ht_destroy(htnode);
//end of hotel RBTree funcs headers

//Reservation red black tree functions
rvnode rv_initilize(rvnode, int, int, int, int);
rvnode rv_grandparent(rvnode);
rvnode rv_uncle(rvnode);
rvnode rv_sibling(rvnode);
rvnode rv_findNode(rvtree, int);
void rv_insertNode(rvtree, int, int, int, int);
void rv_insertUtil(rvnode);
void rv_removeNode(rvtree, int);
void rv_removeUtil(rvnode);
void rv_rotateRight(rvnode);
void rv_rotateLeft(rvnode);
rvnode rv_findNode(rvtree, int);
void rv_replaceNode(rvtree, rvnode, rvnode);
void rv_printNode(rvnode n);
void rv_inorderIterator(rvnode n, void (*func)(rvnode));
void rv_inorderPrint(rvtree t);
void rv_destroy(rvnode);
//end of reservation RBTree funcs headers

struct Graph *createGraph(int V);
void addEdge(struct Graph *graph, int src, int dest);
void printGraph(struct Graph *graph);
int dijkstra_cost(struct Graph *graph, int origin, int destination);
int dijkstra_distance(struct Graph *graph, int origin, int destination);
void createSites();

extern struct Person people[100];
//array of st_nodes
extern struct st_node st_arr[100];
//transportation of [src][dest] exists:1, else:0
extern int tp_visit[100][100];
//tp_arr[src site num][dest site num]
extern struct tp_node tp_arr[100][100];