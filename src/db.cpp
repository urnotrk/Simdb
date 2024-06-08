#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../include/parse.h"

#define TABLE_MAX_PAGES 100


typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;


typedef struct {
  uint32_t num_rows;
  void* pages[TABLE_MAX_PAGES];
} Table;


Table* new_table() {
  Table* table = (Table*)malloc(sizeof(Table));
  table->num_rows = 0;
  for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
      table->pages[i] = NULL;
  }
  return table;
}

InputBuffer* new_input_buffer() {
  InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

void print_prompt() { printf("db > "); }

void read_input(InputBuffer* input_buffer) {
  ssize_t bytes_read =
      getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

  if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  // Ignore trailing newline
  input_buffer->input_length = bytes_read - 1;
  input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}


MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}

void DB() {
    Table* table = new_table();
    InputBuffer* input_buffer = new_input_buffer();
    while (true) {
        print_prompt();
        read_input(input_buffer);
            
        if (input_buffer->buffer[0] == '.') {
        //以.开头 元指令
            switch (do_meta_command(input_buffer)) {
                case (META_COMMAND_SUCCESS):
                continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                printf("Unrecognized command '%s'\n", input_buffer->buffer);
                continue;
            }
        }

        // Statement statement;
        // switch (prepare_statement(input_buffer, &statement)) {
        //     case (PREPARE_SUCCESS):
        //     break;
        //     case (PREPARE_SYNTAX_ERROR):
        //     printf("Syntax error. Could not parse statement.\n");
        //     continue;          
        //     case (PREPARE_UNRECOGNIZED_STATEMENT):
        //     printf("Unrecognized keyword at start of '%s'.\n",
        //             input_buffer->buffer);
        //     continue;
        // }



    }
}