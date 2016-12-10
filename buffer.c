#include "internal.h"
#include "buffer.h"

// Internal structures
typedef struct {
    char *data;
    int pre;
    int past;
    int size;
} Buffer;

// Internal functions

internal void buffer_expand(Buffer *b)
{
    int nsize = b->size * 2;
    
}

// Public functions

BufferHandle buffer_create(int size)
{
    assert(size > 0);
    
    Buffer *b = malloc(sizeof(Buffer));
    if(b == 0) {
        return 0;
    }
    
    b->size = size;
    b->data = malloc(size);
    if(b->data == 0) {
        free(b);
        return 0;
    }
    b->past = 0;
    b->pre = 0;
    
    return b;
}

void buffer_destroy(BufferHandle handle)
{
    Buffer *b = handle;
    
    free(b->data);
    free(b);
}

void buffer_insert(BufferHandle handle, char c)
{
    Buffer *b = handle;
    
    if(b->pre + b->past == b->size) {
        buffer_expand(buffer);
    }
    b->data[b->pre] = c;
    b->pre++;
}