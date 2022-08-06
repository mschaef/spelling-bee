#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 64

typedef uint64_t letter_mask;

letter_mask letter_to_mask(char letter) {
     assert(isalpha(letter));

     return 0x1 << (toupper(letter) - 'A');
}

letter_mask letters_to_mask(char *letters) {
     letter_mask m = 0;

     while (*letters) {
          char l = *letters++;

          if (isalpha(l)) {
               m |= letter_to_mask(l);
          } else {
               fprintf(stderr, "Invalid letter for lettermask: %c\n", l);
          }
     }

     return m;
}

void scan_wordlist(letter_mask central_letter_mask, letter_mask board_mask) {
     FILE *f = fopen("wordlist-large", "r");

     char buf[BUF_SIZE];

     while(fgets(buf, BUF_SIZE, f)) {
          size_t len =  strlen(buf) - 1;
          buf[len] = '\0';

          if (len < 4) {
               continue;
          }

          letter_mask word_letters = letters_to_mask(buf);

          if (word_letters & ~board_mask) {
               continue;
          }

          if (word_letters & central_letter_mask) {
               printf("%s\n", buf);
          }
     }

     fclose(f);
}

int main(int argc, char *argv[]) {
     if (argc < 2) {
          fprintf(stderr, "Usage: %s <board-letters>\n", argv[0]);
          return 1;
     }

     char *letters = argv[1];

     scan_wordlist(letter_to_mask(letters[0]), letters_to_mask(letters));

     return 0;
}
