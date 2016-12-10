#include "internal.h"
#include "buffer.h"

// Internal structures
typedef struct {
    char *data;
    int pre;
    int post;
    int size;
} Buffer;

// Internal functions

internal void buffer_expand(Buffer *b)
{
    int nsize = b->size * 2;
    char *ndata = malloc(nsize);
    
    for(int i = 0; i < b->pre; i++) {
        ndata[i] = b->data[i];
    }
    
    for(int i = 0; i < b->post; i++) {
        ndata[nsize-i-1] = b->data[b->size-i-1];
    }
    
    b->size = nsize;
    free(b->data);
    b->data = ndata;
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
    b->post = 0;
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
    
    if(b->pre + b->post == b->size) {
        buffer_expand(b);
    }
    b->data[b->pre] = c;
    b->pre++;
}

void buffer_insert_string(BufferHandle handle, const char *str)
{
    assert(str);
    while(*str) {
        buffer_insert(handle, *str++);
    }
}

void buffer_move_left(BufferHandle handle)
{
    Buffer *b = handle;
    
    if(b->pre > 0) {
        char c = b->data[b->pre-1];
        b->data[b->size - b->post - 1] = c;
        b->pre--;
        b->post++;
    }
}

void buffer_move_right(BufferHandle handle)
{
    Buffer *b = handle;
    
    if(b->post > 0) {
        b->data[b->pre] = b->data[b->size - b->post];
        b->pre++;
        b->post++;
    }
}

void buffer_move(BufferHandle handle, int distance)
{
    if(distance > 0) {
        for(int i = 0; i < distance; i++) {
            buffer_move_right(handle);
        }
    } else {
        distance = -distance;
        for(int i = 0; i < distance; i++) {
            buffer_move_left(handle);
        }
    }
}

void buffer_seek(BufferHandle handle, int pos)
{
    Buffer *b = handle;
    buffer_move(handle, pos - b->pre);
}

char* buffer_stringify(BufferHandle handle)
{
    Buffer *b = handle;
    
    int size = b->pre + b->post;
    char *str = malloc(size+1);
    str[size] = 0;
    
    for(int i = 0; i < b->pre; i++) {
        str[i] = b->data[i];
    }
    
    for(int i = 0; i < b->post; i++) {
        str[size-i-1] = b->data[b->size-i-1];
    }
    
    return str;
}