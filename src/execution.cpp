#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "../include/common.h"


void serialize_row(Row* source, void* destination,void* node) {
  printf("after4-----%d\n",*(leaf_node_num_cells(node)));

  // char* dest = static_cast<char*>(destination);
  memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
  printf("after5-----%d\n",*(leaf_node_num_cells(node)));

  // memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
  // memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
  strncpy((char*)destination + USERNAME_OFFSET, source->username, USERNAME_SIZE);
  printf("after6-----%d\n",*(leaf_node_num_cells(node)));

  strncpy((char*)destination + EMAIL_OFFSET, source->email, EMAIL_SIZE);
  printf("after7-----%d\n",*(leaf_node_num_cells(node)));

}

void deserialize_row(void* source, Row* destination) {
  // char* sour = static_cast<char*>(source);
  memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
  memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
  memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

//The get_page() method has the logic for handling a cache miss. We assume pages are saved one after the other in the database file: Page 0 at offset 0, page 1 at offset 4096, page 2 at offset 8192, etc. If the requested page lies outside the bounds of the file, we know it should be blank, so we just allocate some memory and return it. The page will be added to the file when we flush the cache to disk later
void* get_page(Pager* pager, uint32_t page_num) {
  if (page_num > TABLE_MAX_PAGES) {
    printf("Tried to fetch page number out of bounds. %d > %d\n", page_num,
           TABLE_MAX_PAGES);
    exit(EXIT_FAILURE);
  }

  if (pager->pages[page_num] == NULL) {
    // Cache miss. Allocate memory and load from file.
    printf("---in get page---null-malloc------\n");
    void* page = malloc(PAGE_SIZE);
    uint32_t num_pages = pager->file_length / PAGE_SIZE;
    printf("----in get page-----num_pages:%d------\n",num_pages);
    // We might save a partial page at the end of the file
    if (pager->file_length % PAGE_SIZE) {
      num_pages += 1;
    }

    if (page_num <= num_pages) {
      lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
      ssize_t bytes_read = read(pager->file_descriptor, page, PAGE_SIZE);
      if (bytes_read == -1) {
        printf("Error reading file: %d\n", errno);
        exit(EXIT_FAILURE);
      }
    }

    pager->pages[page_num] = page;

    if (page_num >= pager->num_pages) {
      pager->num_pages = page_num + 1;
    }
  }
  printf("--------in get page------before-return---node:%d--\n",*leaf_node_num_cells(pager->pages[page_num]));
  return pager->pages[page_num];
}


// void* row_slot(Table* table, uint32_t row_num) {
void* cursor_value(Cursor* cursor) {
  uint32_t page_num = cursor->page_num;
  void* page = get_page(cursor->table->pager, page_num);
  return leaf_node_value(page, cursor->cell_num);
}


void cursor_advance(Cursor* cursor) {
  uint32_t page_num = cursor->page_num;
  void* node = get_page(cursor->table->pager, page_num);

  cursor->cell_num += 1;
  if (cursor->cell_num >= (*leaf_node_num_cells(node))) {
     cursor->end_of_table = true;
   }
}

void print_row(Row* row) {
  printf("| %-10u | %-32s | %-32s |\n", row->id, row->username, row->email);
}

void leaf_node_insert(Cursor* cursor, uint32_t key, Row* value) {
  void* node = get_page(cursor->table->pager, cursor->page_num);

  uint32_t num_cells = *leaf_node_num_cells(node);
  if (num_cells >= LEAF_NODE_MAX_CELLS) {
    // Node full
    printf("Need to implement splitting a leaf node.\n");
    exit(EXIT_FAILURE);
  }

  if (cursor->cell_num < num_cells) {
    // Make room for new cell
    for (uint32_t i = num_cells; i > cursor->cell_num; i--) {
      memcpy(leaf_node_cell(node, i), leaf_node_cell(node, i - 1),
             LEAF_NODE_CELL_SIZE);
    }
  }

  *(leaf_node_num_cells(node)) += 1;
  printf("after1-----%d\n",*(leaf_node_num_cells(node)));
  printf("-------------------\n\n");
  *(leaf_node_key(node, cursor->cell_num)) = key;
  printf("after2-----%d\n",*(leaf_node_num_cells(node)));
  printf("-------------------\n\n");
  void * temp = leaf_node_value(node, cursor->cell_num);
  printf("after3-----%d\n",*(leaf_node_num_cells(node)));
  printf("-------------------\n\n");
  serialize_row(value, temp,node);
  printf("-------------------\n\n");
  printf("after8-----%d\n",*(leaf_node_num_cells(node)));
}


ExecuteResult execute_insert(Statement* statement, Table* table) {
  void* node = get_page(table->pager, table->root_page_num);
  printf("exe-------%d\n",*leaf_node_num_cells(node));
  if ((*leaf_node_num_cells(node) >= LEAF_NODE_MAX_CELLS)) {
     return EXECUTE_TABLE_FULL;
   }

  Row* row_to_insert = &(statement->row_to_insert);
  Cursor* cursor = table_end(table);
  leaf_node_insert(cursor, row_to_insert->id, row_to_insert);
  printf("----------after leaf_node_insert : node:%d",*leaf_node_num_cells(get_page(cursor->table->pager, cursor->page_num)));
  free(cursor);

  return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement* statement, Table* table) {
  Cursor* cursor = table_start(table);
  Row row;
  while (!(cursor->end_of_table)) {
    deserialize_row(cursor_value(cursor), &row);
    print_row(&row);
    cursor_advance(cursor);
  }

  free(cursor);
  return EXECUTE_SUCCESS;
}



ExecuteResult execute_statement(Statement* statement, Table* table) {
  switch (statement->type) {
    case (STATEMENT_INSERT):
      // printf("This is where we would do an insert.\n");
      return execute_insert(statement, table);
      break;
    case (STATEMENT_SELECT):
      // printf("This is where we would do a select.\n");
      return execute_select(statement, table);
      break;
  }
}