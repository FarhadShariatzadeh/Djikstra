#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <float.h>
#include "Djikstra.h"

////////////////////Graph///////////////////////////

Graph* CreateGraph() {
  Graph* graph = (Graph*)malloc(sizeof(Graph));
  graph->num_nodes = 0;
  return graph;
}

City* CreateCity(char* name, int dist) {
  City* city = (City*)malloc(sizeof(City));
  char* cityName = (char*)malloc(sizeof(char));
  strcpy(cityName, name);
  city->name = cityName;
  city->dist = dist;
  return city;
}

Node* CreateNode(City* city) {
  Node* node = (Node*)malloc(sizeof(Node));
  node->city = city;
  node->next = NULL;
  return node;
}

void AddNode(Graph* graph, Node* node) {
  // printf("num_nodes: %d \n", graph->num_nodes);
  graph->nodes[graph->num_nodes++] = node;
  // printf("City of %s was added to the graph. \n", node->city->name);
  // printf("num_nodes: %d \n", graph->num_nodes);
}

void AddNeighbor(Node* src, Node* dest) {
  Node* p = src;
  while (p->next != NULL) {
    p = p->next;
  }
  p->next = dest;
  // printf("City of %s with the distance of %d was added as a neighbor to
  // the city of %s. \n", p->next->city->name, p->next->city->dist,
  // src->city->name);
}

void AddDestinationToGraph(Graph* graph, Node* node) {
  int i;
  for (i = 0; i < graph->num_nodes; i++) {
    if (strcmp(graph->nodes[i]->city->name, node->city->name) == 0) {
      return;
    }
  }
  City* newCity = CreateCity(node->city->name, node->city->dist);
  Node* newNode = CreateNode(newCity);
  AddNode(graph, newNode);
  return;
}

void AddEntryToGraph(Graph* graph, char* src, char* dest, int dist) {
  char* new_srcName = (char*)malloc(sizeof(char));
  strcpy(new_srcName, src);
  char* new_destName = (char*)malloc(sizeof(char));
  strcpy(new_destName, dest);
  // printf("Entered to AddEntryToGraph \n");
  City* destCity = CreateCity(new_destName, dist);
  Node* destNode = CreateNode(destCity);
  // printf("Destination--> %s Node was generated. \n", destNode->city->name);
  int i;
  for (i = 0; i < graph->num_nodes; i++) {
    // printf("%s is getting checked with %s !!
    // \n", graph->nodes[i]->city->name, new_word);
    if (strcmp(graph->nodes[i]->city->name, new_srcName) == 0) {
        // printf("%s was already added to the graph ->
        // skipped adding it. \n", src);
        AddNeighbor(graph->nodes[i], destNode);
        AddDestinationToGraph(graph, destNode);
        return;
    }
  }
  City* srcCity = CreateCity(src, 0);
  Node* srcNode = CreateNode(srcCity);
  // printf("Source--> %s Node was generated. \n", srcNode->city->name);
  AddNeighbor(srcNode, destNode);
  AddNode(graph, srcNode);
  AddDestinationToGraph(graph, destNode);
  return;
}

void DestroyNode(Node* node) {
  // Node* p = node;
  // Node* n = p->next;
  while (node->next != NULL) {
    free(node->city->name);
    free(node->city);
    free(node);
    // p = n;
    // n = n->next;
  }
}

void DestroyGraph(Graph* graph) {
  int i;
  for (i = 0; i < graph->num_nodes; i++) {
    free(graph->nodes[i]);
  }
}

/////////////// Dijkstra ////////////////////////////

int FindIndex(Graph* graph, char* name) {
  int i;
  for (i = 0; i < graph->num_nodes; i++) {
    if (strcmp(graph->nodes[i]->city->name, name) == 0) {
      return i;
    }
  }
  return -1;
}

Node* FindInGraph(Graph* graph, char* cityName) {
  int i;
  for (i = 0; i < graph->num_nodes ; i++) {
    if (strcmp(graph->nodes[i]->city->name, cityName) == 0) {
      return graph->nodes[i];
    }
  }
  return NULL;
}

Node* FindInNeighbors(Node* src, char* destName) {
  Node* p = src;
  while (p != NULL) {
    if (strcmp(p->city->name, destName) == 0) {
      return p;
    }
    p = p->next;
  }
  return NULL;
}

int FindUnvisitedNearest(int* distance, int* visited, int size) {
  int min = INT_MAX;
  int index = -1;
  int i;
  for (i = 0; i < size; i++) {
    if (distance[i] < min && visited[i] == 0) {
      min = distance[i];
      index = i;
    }
  }
  // printf("Next min distance is: %d \n", distance[index]);
  return index;
}

double HasPath(Node* node, char* otherCity) {
  // printf("quick check city is %s  \n", node->city->name);
  Node* p = node->next;
  // printf("Other city is: %s \n", otherCity);
  // printf("from %s: \n", p->city->name);
  while (p != NULL) {
    // printf("to %s \n", p->city->name);
    // printf("is:----------------> %d \n", p->city->dist);
    if (strcmp(p->city->name, otherCity) == 0) {
      int res = p->city->dist;
      // printf("hasPath to return is: %d \n", res);
      return res;
    }
    p = p->next;
  }
  return -1;
}

void FillArray(Graph* graph, int* arr, Node* node) {
  City* newCity = CreateCity(node->city->name, node->city->dist);
  Node* newNode = CreateNode(newCity);
  arr[FindIndex(graph, node->city->name)] = newNode->city->dist;
  }

void PrintArray(int* arr, int size) {
  int h;
  for (h = 0; h < size; h++) {
    printf("%d ", arr[h]);
  }
  printf("\n");
}

void PrintPath(Node* src) {
  Node* p = src;
  int first = 1;
  while (p != NULL) {
    if (first == 1) {
      printf("The shortest path is: %s", p->city->name);
      p = p-> next;
      first = 0;
      continue;
    }
    printf("  --> %s", p->city->name);
    p = p-> next;
  }
  printf("\nTotal distance is: %d \n", src->city->dist);
}


Node* FindPath(Graph* graph, int* distance, int* originalParents,
char* src, char* dest) {
  int size = graph->num_nodes;
  int* parents = malloc(size);
  int srcIndex = FindIndex(graph, src);
  int destIndex = FindIndex(graph, dest);
  int i;
  for (i = 0; i < size; i++) {
    parents[i] = originalParents[i];
  }
  City* newDestCity = CreateCity(graph->nodes[destIndex]->city->name,
  distance[destIndex]);
  int totalDist = distance[destIndex];
  Node* newDestNode = CreateNode(newDestCity);
  Node* pointer = newDestNode;
  int crr = parents[destIndex];
  int done = 0;
  while (done == 0) {
     City* nextCity = CreateCity(graph->nodes[crr]->city->name, distance[crr]);
     Node* nextNode = CreateNode(nextCity);
     nextNode->next = pointer;
     pointer = nextNode;
     if (strcmp(graph->nodes[crr]->city->name, src) == 0) {
        done = 1;
        pointer->city->dist = totalDist;
        // since this is the source node, we know its distance to the 
        // itself is 0, so we put the total distance in its city's dist
        // property so we can return it within the final node to the
        // caller of this method.
    }
     crr = parents[crr];
  }
  PrintPath(pointer);
  return pointer;
}

int* distance;
int* parents;

void ShortestPath(Graph* graph, char* srcName, char* destName) {
  char* src = (char*)malloc(sizeof(char));
  strcpy(src, srcName);
  char* dest = (char*)malloc(sizeof(char));
  strcpy(dest, destName);


  // printf("Entered Shortestpath. \n");
  int size = graph->num_nodes;
  // printf("Size of the graph is: %d. \n", size);
  Node* srcNode = FindInGraph(graph, src);
  // printf("%s source node was found in the graph", srcNode->city->name);
  int* visited = malloc(size);
  distance = malloc(size);
  parents = malloc(size);
  int i;
  for (i = 0; i < size; i++) {
    distance[i] = INT_MAX;
    visited[i] = 0;
    parents[i] = -1;
  }
  // PrintArray(distance, size);
  // printf("Arrays we created. \n");
  FillArray(graph, distance, srcNode);
  // printf("Array distance were filled. \n");
  int visitedCount = 0;
  int srcIndex = FindIndex(graph, src);
  // printf("The index of the source node in the graph is: %d. \n", srcIndex);
  distance[srcIndex] = 0;
  parents[srcIndex] = 0;
  printf("PRINTING GRAPH NODES: \n");

  int h;
  for (h = 0; h < size; h++) {
    printf("%s ", graph->nodes[h]->city->name);
  }
  printf("\n");
  while (visitedCount < size) {
    // PrintArray(distance, size);
    // printf("\n");
    // printf("Entered the while loop. \n");
    int index = FindUnvisitedNearest(distance, visited, size);
    // printf("Calcualted index is: %d \n", index);
    if (index == -1) {
      break;
    }
    Node* g = graph->nodes[index];
    while (g != NULL) {
      // printf("%s->",g->city->name);
      g = g->next;
    }
    // printf("\n");

    // printf("The graph-index of the nearest city is: %d. \n", index);
    visitedCount++;
    int j;
    for (j = 0; j < size; j++) {
      if (srcIndex == j) {
        continue;
      }
      // printf("Index %d is being visited. \n", j);
      int hasPath = HasPath(graph->nodes[index],
      graph->nodes[j]->city->name);
      // printf("HasPath value is: %d \n", hasPath);
      if (hasPath == -1) {
          continue;
      }
      // printf("Current hasPath is: %d \n", hasPath);
      if (visited[index] == 0 &&
      distance[index] != INT_MAX &&
      distance[index] + hasPath < distance[j]) {
         distance[j] = distance[index] + hasPath;
         parents[j] = index;
         // printf("Final calculation for j=%d is: \n distance[j](%d) =
         // distance[index](%d) + hasPath(%d) \n", j, distance[j], distance[index], hasPath);
      }
      // printf("\n \n");
    }
    visited[index] = 1;
  }

  // printf("End of shortest Path calculation. \n");
  // PrintArray(parents, size);
}

///////////////////////// Main /////////////////////////////

int main() {
  Graph* graph = CreateGraph();
  FILE * fp;
  char line[WORD_SIZE];
  fp = fopen("mini_miles_edges.txt", "r");

  while (fgets(line, WORD_SIZE, fp) != NULL) {
    char delim[] = " ";
    char *ptr = strtok(line, delim);
    char* array[3];
    int i = 0;
    while (ptr != NULL) {
      array[i++]=ptr;
      ptr = strtok(NULL, delim);
      }

  // printf("\n");
  // printf("adding distance is: %d \n", atoi(array[2]));

  AddEntryToGraph(graph, array[0], array[1], atoi(array[2]));
  }

  ShortestPath(graph, "Seattle_WA", "Boston_MA");
  Node* src = FindPath(graph, distance, parents, "Seattle_WA", "Austin_TX");
  DestroyGraph(graph);
  return 0;
}
