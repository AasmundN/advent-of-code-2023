#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

const char *numbers[] = {
  "one",
  "two",
  "three",
  "four",
  "five",
  "six",
  "seven",
  "eight",
  "nine" 
}; 

char match_char(FILE *fptr, char *start_char) {
  char ch = *start_char;
  if (!isalpha(ch)) return '\0';

  for (int i = 0; i < sizeof(numbers) / sizeof(char*); i++) {
    int shift = 0;
    while (numbers[i][shift] == ch) {
      if (shift == strlen(numbers[i]) - 1) { 
        fseek(fptr, -shift, SEEK_CUR);
        return (i + 1) + '0';      
      } 
      ch = fgetc(fptr);
      shift++;
    }
    fseek(fptr, -shift, SEEK_CUR);
    ch = *start_char;
  } 
  return '\0';
}

int main() {
  FILE *fptr;
  fptr = fopen("input.txt", "r");

  int sum = 0;

  char num[3];
  num[0] = '\0';

  char ch;

  while (!feof(fptr)) {
    ch = fgetc(fptr);   
    if (ch == '\n') {
      sum += atoi(num);
      num[0] = '\0';
      continue;
    }

    char digit = '\0';
    
    if (isdigit(ch)) digit = ch; 
    else digit = match_char(fptr, &ch); 

    if (digit == '\0') continue;

    if (num[0] == '\0') num[0] = digit;
    num[1] = digit; 
  }

  printf("Sum: %d\n", sum);
  fclose(fptr);
}
