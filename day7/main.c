#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_NUM_HANDS 1100
#define MAX_LINE_LENGTH 20
#define HAND_SIZE 5

// all available cards
char cards[] = {"AKQJT98765432"};

typedef enum {
  FIVE_OF_A_KIND,
  FOUR_OF_A_KIND,
  FULL_HOUSE,
  THREE_OF_A_KIND,
  TWO_PAIR,
  ONE_PAIR,
  HIGH_CARD
} Hand_t;

typedef struct {
  char cards[HAND_SIZE + 1];
  int bid;
} Hand;

Hand **get_hands(char *file) {
  FILE *fptr;
  fptr = fopen(file, "r");

  Hand **hands = malloc(MAX_NUM_HANDS * sizeof(Hand*));

  char line[MAX_LINE_LENGTH];
  int hand_index = 0;
 
  // read hands from file
  while (fgets(line, MAX_LINE_LENGTH, fptr)) {
    // create hand struct
    hands[hand_index] = malloc(sizeof(Hand*));
    strncpy(hands[hand_index]->cards, line, 5); 
    hands[hand_index]->bid = strtol(&line[6], NULL, 10); 

    hand_index++;
  } 

  // reallocate memory for hands array
  hands = realloc(hands, (hand_index + 2) * sizeof(Hand*));
  hands[hand_index + 1] = NULL;
  
  fclose(fptr);
  return hands;
}

int get_card_index(char card) {
  int card_index = 0;
  while (cards[card_index] != card) card_index++;
  return card_index;
}

Hand_t get_hand_type(char *hand) {
  int *card_counters = calloc(sizeof(cards) - 1, sizeof(int));
  
  // count cards
  for (int i = 0; i < HAND_SIZE; i++) {
    card_counters[get_card_index(hand[i])]++;
  }

  int pairs = 0;
  int tripple = 0;

  // find hand type identifiers
  for (int i = 0; i < sizeof(cards) - 1; i++) {
    switch (card_counters[i]) {
      case 5:
        free(card_counters);
        return FIVE_OF_A_KIND;
        break;
      
      case 4:
        free(card_counters);
        return FOUR_OF_A_KIND;
        break;

      case 3:
        tripple = 1;
        break;

      case 2:
        pairs++;
        break;
    }
  } 
 
  free(card_counters);

  if (pairs == 1 && tripple == 1) return FULL_HOUSE;
  else if (tripple == 1) return THREE_OF_A_KIND;
  else if (pairs == 2) return TWO_PAIR;
  else if (pairs == 1) return ONE_PAIR;
  else return HIGH_CARD; 
}

// return true if hand one is better than hand two
bool compare_hands(char *hand_one, char *hand_two) {
  Hand_t type_one = get_hand_type(hand_one);
  Hand_t type_two = get_hand_type(hand_two);

  if (type_one < type_two) return true;
  else if (type_one > type_two) return false;  

  for (int i = 0; i < HAND_SIZE; i++) {
    // find card index in cards array
    int index_one = get_card_index(hand_one[i]);
    int index_two = get_card_index(hand_two[i]);

    if (index_one < index_two) return true;
    else if (index_one > index_two) return false;
  }

  return false;
}

void merge(Hand **hands, int start, int split, int end) {
  // we are dealing with a single element, no changes needed
  if (start == end) return;

  // create temporary arrays
  Hand *subarray_one = calloc((split - start) + 1, sizeof(Hand));
  Hand *subarray_two = calloc((end - split + 1) + 1, sizeof(Hand));

  // fill temporary arrays
  for (int i = 0; i < split - start; i++) {
    strncpy(subarray_one[i].cards, hands[i + start]->cards, 5);
    subarray_one[i].bid = hands[i + start]->bid;
  }

  for (int i = 0; i < end - split + 1; i++) {
    strncpy(subarray_two[i].cards, hands[i + split]->cards, 5);
    subarray_two[i].bid = hands[i + split]->bid;
  }

  // indexes for subarray one and two respectively
  int index_one = 0;
  int index_two = 0;  

  // merge subarrays
  for (int i = start; i < end + 1; i++) {
    bool index = 
      compare_hands(subarray_one[index_one].cards, subarray_two[index_two].cards); 

    if (index) {
      strncpy(hands[i]->cards, subarray_one[index_one].cards, 5);
      hands[i]->bid = subarray_one[index_one].bid;
      index_one++;
    } else {
      strncpy(hands[i]->cards, subarray_two[index_two].cards, 5);
      hands[i]->bid = subarray_two[index_two].bid;
      index_two++;
    }
  }

  free(subarray_one);
  free(subarray_two);
}

// start and end are the first and last index of the subarray
void merge_sort(Hand **hands, int start, int end) {
  // find split index
  int split = start + ((end - start) + 1) / 2; 

  // split array until each subarray is of length one
  if (end - start > 1) {
    merge_sort(hands, start, split - 1);
    merge_sort(hands, split, end);
  } 
 
  // merge subarrays 
  merge(hands, start, split, end); 
}

int main() {
  Hand **hands = get_hands("input.txt");

  // find length of hand array
  int hands_length = 0;
  while (hands[hands_length] != NULL) hands_length++;

  // sort hands
  merge_sort(hands, 0, hands_length - 1);  

  unsigned long long total = 0;

  // loop over hands
  int hand_index = 0;
  while (hands[hand_index] != NULL) {
    total += (hands_length - hand_index) * hands[hand_index]->bid;
    free(hands[hand_index]);
    hand_index++;
  }
  free(hands);

  printf("Length: %d, Total winnings: %llu\n", hand_index, total);
  return 0;
}
