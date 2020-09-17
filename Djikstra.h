
#define WORD_SIZE 50
#define MAX_NUM_NODES 1000
#define DJIKSTRA_H


typedef struct node Node;
typedef struct city City;

struct node {
  City* city;
  Node* next;
};

struct city {
  char* name;
  int dist;
};

typedef struct graph {
  Node* nodes[MAX_NUM_NODES];
  int num_nodes;
} Graph;


