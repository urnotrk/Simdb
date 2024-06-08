#ifndef PARSE_H
#define PARSE_H


#include <stdint.h>
#include "db.h"


#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255


typedef enum { 
  STATEMENT_INSERT, 
  STATEMENT_SELECT 
} StatementType;


typedef struct {
  uint32_t id;
  char username[COLUMN_USERNAME_SIZE];
  char email[COLUMN_EMAIL_SIZE];
} Row;



typedef struct {
  StatementType type;
  Row row_to_insert;
} Statement;



typedef enum { 
  PREPARE_SUCCESS,
  PREPARE_SYNTAX_ERROR,
  PREPARE_UNRECOGNIZED_STATEMENT 
} PrepareResult;

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement);

#endif //PARSE_H