#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "iss.h"


int main(int argc, char **argv) {
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s [filename].\n", argv[0]);
    exit(1);
  }

  FILE *fp = fopen(argv[1], "rb");
  if (fp == NULL)
  {
    fprintf(stderr, "Error during fopen.\n");
    exit(1);
  }
  fseek(fp, 0, SEEK_END);
  uint16_t size = ftell(fp);
  rewind(fp);

  char *mem = malloc(size);

  if (fread(mem, 1, size, fp) != size)
  {
    fprintf(stderr, "Error during fread.\n");
    exit(1);
  }
  fclose(fp);
  cpu(mem);
  free(mem);
  return 0;
}
