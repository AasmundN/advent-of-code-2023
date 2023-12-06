#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int is_winner(char *line, int num) {
  char *ch = strchr(line, ':');

  while (*ch != '|') {
    if (!isdigit(*ch)) {
      ch++;
      continue;
    }
    int winning_num = strtol(ch, &ch, 10);
    if (num == winning_num) return 1; 
  }    

  return 0;
}

int main() {
  FILE *fptr;
  fptr = fopen("input.txt", "r");

  int sum = 0;
  char line[200];

  while(fgets(line, sizeof(line), fptr)) {
    char *ch = strchr(line, '|');
    int matches = 0;
    
    while (*ch != '\n') {
      if (!isdigit(*ch)) {
        ch++;
        continue;
      }
      int num = strtol(ch, &ch, 10);
      if (is_winner(line, num)) matches++;
    }    
    if (matches) sum += pow(2, matches - 1);
  }

  printf("Sum: %d\n", sum); 
}
