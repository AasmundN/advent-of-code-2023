#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main() {
  FILE *fptr;
  fptr = fopen("input.txt", "r");
  
  int sum = 0;

  char firstChar = '\0';
  char lastChar = '\0';

  char digit[3];
  char c;
  
  for (c = getc(fptr); c != EOF; c = getc(fptr)) {
    if (c == '\n') {
      digit[0] = firstChar;
      digit[1] = lastChar;
      digit[2] = '\0';

      sum += atoi(digit);

      firstChar = '\0';
      lastChar = '\0';
    }
    if (isdigit(c) == 0) continue;
    if (firstChar == '\0') firstChar = c;
    lastChar = c;
  }

  printf("Sum: %d\n", sum);

  fclose(fptr);
}
