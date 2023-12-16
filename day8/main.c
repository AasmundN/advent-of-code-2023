#include <stdio.h>
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

int get_node(Node **nodes, char *node) {
  int node_index = 0;

  // find node
  while (nodes[node_index] != NULL) {
    if (!strcmp(node, nodes[node_index]->el)) break;
    node_index++;
  }

  return node_index;
}

int main() {
  FILE *fptr;
  fptr = fopen("input.txt", "r");

  char *directions = get_directions(fptr);
  Node **nodes = get_nodes(fptr);

  int steps = 0;
  int direction_index = 0;
  int node_index = get_node(nodes, "AAA");

  while (strcmp(nodes[node_index]->el, "ZZZ")) {
    char direction = directions[direction_index];
    char next_node[4] = {'\0'};

    switch (direction) {
      case 'L':
        strncpy(next_node, nodes[node_index]->left, 3);
        break;

      case 'R':
        strncpy(next_node, nodes[node_index]->right, 3);
        break;
    }

    node_index = get_node(nodes, next_node); 
    
    if (directions[direction_index + 1] == '\n') direction_index = 0;
    else direction_index++;

    steps++;
  }

  printf("Steps: %d\n", steps);

  // free nodes memory
  node_index = 0;
  while (nodes[node_index] != NULL) {
    free(nodes[node_index]);
    node_index++;
  }

  free(nodes);
  free(directions);
  
  fclose(fptr);
  
  return 0;
}
