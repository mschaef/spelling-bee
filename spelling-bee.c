#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 64

#define MAX_WORDS 262144

typedef uint64_t letter_mask;

struct word_info {
     char word_letters[BUF_SIZE];
     letter_mask mask;
};

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

size_t load_wordlist(struct word_info words[], size_t max_words) {
     FILE *f = fopen("wordlist-large", "r");

     char buf[BUF_SIZE];

     size_t index = 0;

     while(fgets(buf, BUF_SIZE, f)) {
          size_t len =  strlen(buf) - 1;
          buf[len] = '\0';

          if (len < 4) {
               continue;
          }

          strncpy(words[index].word_letters, buf, BUF_SIZE);
          words[index].mask = letters_to_mask(buf);

          index++;
     }

     fclose(f);

     return index;
}

void scan_wordlist(struct word_info words[], size_t num_words, letter_mask central_letter_mask, letter_mask board_mask) {
     for(size_t index = 0; index < num_words; index++) {
          letter_mask word_letters = words[index].mask;

          if (word_letters & ~board_mask) {
               continue;
          }

          if (word_letters & central_letter_mask) {
               printf("%s\n", words[index].word_letters);
          }
     }
}

struct word_info words[MAX_WORDS];

int main(int argc, char *argv[]) {
     if (argc < 2) {
          fprintf(stderr, "Usage: %s <board-letters>\n", argv[0]);
          return 1;
     }


     size_t num_words = load_wordlist(words, MAX_WORDS);

     char *letters = argv[1];

     scan_wordlist(words, num_words, letter_to_mask(letters[0]), letters_to_mask(letters));

     return 0;
}
