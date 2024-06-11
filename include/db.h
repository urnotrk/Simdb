#ifndef DB_H
#define DB_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TABLE_MAX_PAGES 100

typedef struct {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

//The Pager accesses the page cache and the file. 
//The Table object makes requests for pages through the pager
typedef struct {
  int file_descriptor;
  uint32_t file_length;
  void* pages[TABLE_MAX_PAGES];
} Pager;


typedef struct {
  uint32_t num_rows;
  Pager* pager;
} Table;

void DB(char* filename);
// InputBuffer* new_input_buffer();


#endif //DB_H