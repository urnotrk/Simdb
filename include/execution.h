#ifndef EXEC_H
#define EXEC_H

// #include "db.h"
#include "parse.h"
#include "storage.h"
#include "constants.h"


typedef enum { 
  EXECUTE_TABLE_FULL, 
  EXECUTE_SUCCESS 
} ExecuteResult;

void serialize_row(Row* source, void* destination);
void deserialize_row(void* source, Row* destination);

void* get_page(Pager* pager, uint32_t page_num);
ExecuteResult execute_insert(Statement* statement, Table* table);

ExecuteResult execute_select(Statement* statement, Table* table);


ExecuteResult execute_statement(Statement* statement, Table* table);


#endif //EXEC_H