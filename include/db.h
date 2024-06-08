#ifndef DB_H
#define DB_H

#include <string.h> 
#include <stdlib.h>

typedef struct {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;


void DB();
// InputBuffer* new_input_buffer();


#endif //DB_H