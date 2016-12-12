#ifndef BUFFER_H
#define BUFFER_H

// A handle to the internal buffer structure
typedef void* BufferHandle;

// Creates a new buffer and returns a handle to it. Size has to be > 0
BufferHandle buffer_create(int size);
// Frees the buffer and its data
void buffer_destroy(BufferHandle handle);

// Returns the current offset
int buffer_get_offset(BufferHandle handle);
// Returns the total size
int buffer_get_size(BufferHandle handle);

// Insert a char at the current offset
void buffer_insert(BufferHandle handle, char c);
// Insert a string at the current offset
void buffer_insert_string(BufferHandle handle, const char *str);

// Removes the current char
void buffer_remove(BufferHandle handle);

// Moves the offset one to the left if possible
void buffer_move_left(BufferHandle handle);
// Moves the offset one to the right if possible
void buffer_move_right(BufferHandle handle);
// Move a specified amount, positive beign right, negative left
void buffer_move(BufferHandle handle, int distance);
// Move to a specific offset
void buffer_seek(BufferHandle handle, int pos);

// Moves to the start of the current line
void buffer_seek_home(BufferHandle handle);
// Moves to the end of the current line
void buffer_seek_end(BufferHandle handle);

// Move to the previuos line, trying to keep the offset into the line
void buffer_seek_up(BufferHandle handle);
// Move to the next line, trying to keep the offset into the line
void buffer_seek_down(BufferHandle handle);

// Return a string containg the buffer. Has to be freed with free()
char* buffer_stringify(BufferHandle handle);

#endif /* BUFFER_H */