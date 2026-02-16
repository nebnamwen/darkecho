#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "time.h"
#include "math.h"

#include "map.c"

int main(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    load_map(argv[i]);
  }
}
