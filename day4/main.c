#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int get_line_num(FILE *fptr) {
  int total = 0;
  char ch;

  while (!feof(fptr)) {
    ch = fgetc(fptr);
    if (ch == '\n') total++;
  }
  
  rewind(fptr);
  return total;
}

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

int accumulate_cards(int *winnings, int line_index, int num_lines) {
  int total = winnings[line_index];

  for (int i = line_index + 1; i < line_index + 1 + winnings[line_index]; i++) {
    if (i >= num_lines - 1) break;    
    if (winnings[i]) total += accumulate_cards(winnings, i, num_lines);
  }

  return total;
}

int main() {
  FILE *fptr;
  fptr = fopen("input.txt", "r");

  int points = 0;
  int total_cards = get_line_num(fptr);

  // number of matching numbers for each line
  int winnings[total_cards];
  
  char line[200];
  int line_index = 0;

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

    if (matches) points += pow(2, matches - 1);

    winnings[line_index] = matches;
    line_index++;
  }

  // recursively find all extra cards
  for (int i = 0; i < sizeof(winnings) / sizeof(int); i++) 
    total_cards += accumulate_cards(winnings, i, sizeof(winnings) / sizeof(int));

  printf("Points: %d, Total cards: %d\n", points, total_cards); 
  fclose(fptr);
}
