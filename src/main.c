#include "buffer.h"

int main()
{
    BufferHandle buffer = buffer_create(1);
    
    buffer_insert_string(buffer, "Hello, world!\n");
    buffer_insert_string(buffer, "A second string");
    
    buffer_seek(buffer, 0);
    buffer_insert_string(buffer, "Third line inserted at the end\n");
    
    char *str = buffer_stringify(buffer);
    printf("<START>\n%s\n<END>", str);
    free(str);
    
    buffer_destroy(buffer);
    return 0;
}