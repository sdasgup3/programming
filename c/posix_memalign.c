#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void malloc_alligned(void **memptr, size_t alignment, size_t size) {
  /*
   *  |---------------|---------------|---------------|
   *  Let | denotes the alignment boundary. The goal is to
   *  return the address (memptr) which is a multiple of alignment and to store
   * the actual malloc'd address size_t bytes before memptr.
   *
   *  i.e.
   *  |---------------|---------------|---------------|
   *  ^
   *  |
   *  data = This is the actual pointer returned by malloc.
   *  x = data + sizeof(size_t)
   *  case 1: (x - data) < alignment
   *  |-----x---------|---------------|---------------|
   *                   ^
   *                   |
   *  Find the nearest alignment and store address of Data   sizeof(size_t)
   * before it wastage: alignment bytes
   *
   *  case 2: (x - data) == alignment
   *  |----------------x---------------|---------------|
   *                   ^
   *                   |
   *  x is the alligned position, so return x after  storing the address of data
   * sizeof(size_t) before it wastage: 0 bytes
   *
   *  case 3: (x - data) > alignment
   *  |----------------|--x------------|---------------|
   *                                   ^
   *                                   |
   *  Find the                        nearest alignment and store address of
   * Data   sizeof(size_t) before it wastage: 2*alignment bytes
   */

  size_t total_size = 2 * alignment + size;

  char *data = (char *)malloc(sizeof(char) * total_size);

  char *start_data = data;

  data += sizeof(size_t);

  if (0 == ((size_t)data % alignment)) { // Case 2
    *memptr = (void *)data;
    *((int *)start_data) = (size_t)start_data;
  } else {
    // ofset = distance from x to the next alignment boundary
    size_t offset = alignment - (((size_t)data) % alignment);
    data += offset;
    *memptr = (void *)data;

    assert(0 == (((size_t)(data)) % alignment) && "Alignment problem ");

    *((int *)((char *)data - sizeof(size_t))) = (size_t)start_data;
  }
}

void free_alligned(void *memptr) {
  void *book = ((char *)memptr) - sizeof(size_t);
  void *ptr_to_free = (void *)(*((int *)(book)));
  free(ptr_to_free);
}

int main() {
  void *ptr = NULL;

  int iter = 0;
  srand(time(NULL));

  for (iter = 0; iter < 100; iter++) {
    size_t size = rand() % 10000;
    size_t alignment = rand() % 10000;
    malloc_alligned(&ptr, alignment, size);

    printf("Is byte aligned = %s: aligned %d, size %d\n",
           (((size_t)ptr) % alignment) ? "no" : "yes", alignment, size);

    free_alligned(ptr);
  }
  return 0;
}
