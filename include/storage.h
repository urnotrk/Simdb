#ifndef STORAGE_H
#define STORAGE_H


#include <stdint.h>
#include <stdbool.h>
#include "constants.h"


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


typedef struct {
  Table* table;
  uint32_t row_num;
  bool end_of_table;  // Indicates a position one past the last element
} Cursor;


Table* db_open(const char* filename);
void db_close(Table* table);

Cursor* table_start(Table* table);
Cursor* table_end(Table* table);

#endif //STORAGE_H