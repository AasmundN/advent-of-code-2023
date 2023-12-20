#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1000 
#define MAX_NODES 1000

typedef struct {
  char el[4];
  char left[4];
  char right[4];
} Node;

char *get_directions(FILE *fptr) {
  char *directions = malloc(MAX_LINE_LENGTH * sizeof(char));
  fgets(directions, MAX_LINE_LENGTH, fptr);
  rewind(fptr);
  return directions;
}

Node **get_nodes(FILE *fptr) {
  Node **nodes = malloc(MAX_NODES * sizeof(Node*));  

  char line[MAX_LINE_LENGTH];

  // move to third line;
  fgets(line, MAX_LINE_LENGTH, fptr);
  fgets(line, MAX_LINE_LENGTH, fptr);

  int node_index = 0;

  while (fgets(line, MAX_LINE_LENGTH, fptr)) {
    nodes[node_index] = malloc(sizeof(Node));

    strncpy(nodes[node_index]->el, &line[0], 3);
    strncpy(nodes[node_index]->left, &line[7], 3);
    strncpy(nodes[node_index]->right, &line[12], 3);
   
    node_index++;
  }
  
  nodes = realloc(nodes, (node_index + 2) * sizeof(Node*));
  nodes[node_index + 1] = NULL;

  rewind(fptr);

  return nodes;
}

int *get_starting_nodes(Node **nodes) {
  int *starting_nodes = malloc(MAX_NODES * sizeof(int));

  int node_index = 0;
  int starting_node_index = 0;

  // find starting nodes 
  while (nodes[node_index] != NULL) {
    if (nodes[node_index]->el[2] == 'A') { 
      starting_nodes[starting_node_index] = node_index;
      starting_node_index++;
    }

    node_index++;
  }

  starting_nodes = realloc(starting_nodes, (starting_node_index + 1) * sizeof(int));
  starting_nodes[starting_node_index] = -1;

  return starting_nodes;
}

int find(Node **nodes, char *node) {
  int index = 0;
  while(strcmp(nodes[index]->el, node)) index++;  

  return index;
}

int get_loop_size(Node **nodes, char *directions, int starting_node) {
  int steps = 0;

  int direction_index = 0;
  int node_index = starting_node;

  while (true) {
    if (nodes[node_index]->el[2] == 'Z') break;

    char next_node[4] = {'\0'};

    if (directions[direction_index] == 'R')
      strncpy(next_node, nodes[node_index]->right, 3);
    else
      strncpy(next_node, nodes[node_index]->left, 3);

    node_index = find(nodes, next_node);

    if (directions[direction_index + 1] == '\n') direction_index = 0;
    else direction_index++;

    steps++;
  }  

  return steps;
}

unsigned long long int gcd(unsigned long long int a, unsigned long long int b) {
  unsigned long long int temp;
  while (b != 0) {
    temp = a % b;

    a = b;
    b = temp;
  }

  return a;
}

unsigned long long int lcm(unsigned long long int a, unsigned long long int b) {
  return (a * b) / gcd(a, b);
}

unsigned long long int lcm_array(int *args, size_t size) {
  unsigned long long int temp = args[0]; 

  for (int i = 1; i < size; i++) temp = lcm(temp, args[i]);

  return temp;
}

int main() {
  FILE *fptr;
  fptr = fopen("input.txt", "r");

  char *directions = get_directions(fptr);
  Node **nodes = get_nodes(fptr);

  int num_nodes = 0;
  while (nodes[num_nodes] != NULL) num_nodes++;

  int *starting_nodes = get_starting_nodes(nodes);

  int *loop_sizes = malloc(10 * sizeof(int)); 

  int index = 0;
  while (starting_nodes[index] != -1) {
    loop_sizes[index] = get_loop_size(nodes, directions, starting_nodes[index]); 
    printf("%s: %d\n", nodes[starting_nodes[index]]->el, loop_sizes[index]);
    index++;
  }  

  printf("%llu\n", lcm_array(loop_sizes, index));

  // free nodes memory
  int node_index = 0;
  while (nodes[node_index] != NULL) {
    free(nodes[node_index]);
    node_index++;
  }

  free(loop_sizes);
  free(nodes);
  free(directions);

  fclose(fptr);

  return 0;
}
