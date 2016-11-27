/*************************** HEADER FILES ***************************/
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "sha256.h"

/*********************** FUNCTION DEFINITIONS ***********************/
int main(int argc, char** argv) {
  long bufsize;
  BYTE *source = NULL;
  FILE *fp = fopen(argv[1], "r");

  if (fp != NULL) {
    /* Go to the end of the file. */
    if (fseek(fp, 0L, SEEK_END) == 0) {
      /* Get the size of the file. */
      bufsize = ftell(fp);
      if (bufsize == -1) { /* Error */ }

      /* Allocate our buffer to that size. */
      source = malloc(sizeof(BYTE) * bufsize);

      /* Go back to the start of the file. */
      if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

      /* Read the entire file into memory. */
      size_t newLen = fread(source, sizeof(BYTE), bufsize, fp);
      if ( ferror( fp ) != 0 ) {
        fputs("Error reading file", stderr);
      }
    }
    fclose(fp);
  }



  BYTE resultBuffer[SHA256_BLOCK_SIZE];
  SHA256_CTX context;

  sha256_init(&context);
  sha256_update(&context, source, bufsize);
  sha256_final(&context, resultBuffer);
  
  free(source);

  printf("%s: ", argv[1]);
  for (int i=0; i < SHA256_BLOCK_SIZE; ++i) printf("%x", resultBuffer[i]);
  printf("\n");

  return 0;
}
