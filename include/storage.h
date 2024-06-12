#ifndef STORAGE_H
#define STORAGE_H


#include <stdint.h>

#define TABLE_MAX_PAGES 100

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


Table* db_open(const char* filename);
void db_close(Table* table);

#endif //STORAGE_H