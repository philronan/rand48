#include <stdio.h>
#define TESTING 1

static long rstate = 0;

void srand(unsigned int seed) {
  rstate = (long)seed;
}

int rand(void) {
  rstate = rstate * 214013L + 2531011L;
  return (rstate >> 16) & 0x7fff;
}

#if TESTING == 1

/* (Outputs "38 7719 21238 2437 8855 11797 8365 32285 10450 30612") */
int main(void) {
  int i;
  for (i=0; i<10; i++) {
    printf("%d ",rand());
  }
  putchar('\n');
  return 0;
}

#endif
