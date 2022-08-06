#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include <string.h>

#define BUF_SIZE 64

typedef uint64_t letter_mask;

letter_mask letters_to_mask(char *letters) {
     letter_mask m = 0;

     while (*letters) {
          char l = *letters++;

          if (isalpha(l)) {
               m |= (0x1 << (toupper(l) - 'A'));
          } else {
               fprintf(stderr, "Invalid letter for lettermask: %c\n", l);
          }
     }

     return m;
}

void scan_wordlist(letter_mask central_mask, letter_mask board_mask) {
     FILE *f = fopen("wordlist-large", "r");

     char buf[BUF_SIZE];

     while(fgets(buf, BUF_SIZE, f)) {
          buf[strlen(buf) - 1] = '\0';

          letter_mask word_mask = letters_to_mask(buf);

          if ((word_mask & ~board_mask) != 0) {
               continue;
          }

          if ((word_mask & central_mask) == 0) {
               continue;
          }

          fprintf(stderr, "%s : %llu\n", buf, letters_to_mask(buf));
     }

     fclose(f);
}

int main(int argc, char *argv[]) {
     if (argc < 2) {
          fprintf(stderr, "Usage: %s <board-letters>\n", argv[0]);
          return 1;
     }

     char *letters = argv[1];

     char central[2];
     central[0] = letters[0];
     central[1] = '\0';

     letter_mask central_mask = letters_to_mask(central);
     letter_mask board_mask = letters_to_mask(letters);

     scan_wordlist(central_mask, board_mask);

     fprintf(stderr, "end run %lu.\n", sizeof(letter_mask));
     return 0;
}
