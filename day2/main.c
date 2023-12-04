#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int r = 12;
int g = 13;
int b = 14;

int main() {
  FILE *fptr;
  fptr = fopen("input.txt", "r");

  int sumValidGames = 0, sumPowers = 0;

  char line[200]; 

  while (fgets(line, sizeof(line), fptr)) {
    int id;
    int valid = 1;
    char *endptr;
    int lowestRed = 0, lowestGreen = 0, lowestBlue = 0;

    id = strtol(&line[5], &endptr, 10);
    
    while (*endptr != '\n') {
      int num;

      if (isdigit(*endptr)) {
        num = strtol(endptr, &endptr, 10);
        
        switch (*(endptr + 1)) {
          case 'r':
            if (num > r) valid = 0;
            if (num > lowestRed) lowestRed = num;
            break;

          case 'g':
            if (num > g) valid = 0;
            if (num > lowestGreen) lowestGreen = num;
            break;

          case 'b':
            if (num > b) valid = 0;
            if (num > lowestBlue) lowestBlue = num;
            break;
        }
      } 
      endptr++;
    }
    if (valid) sumValidGames += id;
    sumPowers += lowestRed * lowestGreen * lowestBlue;
  }

  printf("Sum of valid games: %d, Sum of powers: %d\n", sumValidGames, sumPowers); 
  fclose(fptr);
}
