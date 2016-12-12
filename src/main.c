#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "buffer.h"
#include <stdint.h>
#include <stdio.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

BufferHandle buffer;

typedef struct {
    int texture;
    stbtt_bakedchar cdata[96];
    stbtt_fontinfo info;
    int vertical_advance;
} Font;

Font init_font(const char *path, float size)
{
    Font font = {0};
    
    FILE *f = fopen(path, "rb");
    fseek(f, 0, SEEK_END);
    int buffer_size = ftell(f);
    rewind(f);
    unsigned char *buffer = malloc(buffer_size);
    fread(buffer, 1, buffer_size, f);
    fclose(f);
    
    unsigned char *bitmap = malloc(2048*2048);
    
    stbtt_InitFont(&font.info, buffer, 0);
    float scale = stbtt_ScaleForPixelHeight(&font.info, size);
    int ascent, descent, linegap;
    stbtt_GetFontVMetrics(&font.info, &ascent, &descent, &linegap);
    font.vertical_advance = scale * (ascent - descent + linegap);
    
    int bake_ret = stbtt_BakeFontBitmap(buffer, 0, size, bitmap, 2048, 2048, 32, 96, font.cdata);
    free(buffer);
    
    glGenTextures(1, &font.texture);
    glBindTexture(GL_TEXTURE_2D, font.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 2048, 2048, 0, GL_ALPHA, GL_UNSIGNED_BYTE, bitmap);
    free(bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    return font;
}

void draw_buffer(Font font, BufferHandle buffer)
{
    float x = 0;
    float y = font.vertical_advance;
    
    const char *ptr = buffer_stringify(buffer);
    int offset = 0;
    
    glBindTexture(GL_TEXTURE_2D, font.texture);
    while(*ptr) {
        if((*ptr == '\n') || (*ptr >= 32 && *ptr < 128)) {
            stbtt_aligned_quad q;
            float old_y = y;
            float old_x = x;
            
            if(*ptr == '\n') {
                stbtt_GetBakedQuad(font.cdata, 2048, 2048, '_'-32, &x, &y, &q, 1);
                y += font.vertical_advance;
                x = 0;
                if(offset == buffer_get_offset(buffer)) {
                    glDisable(GL_BLEND);
                    glBegin(GL_QUADS);
                    glColor4f(0, 1, 0, 1);
                    glVertex2f(q.x0, old_y);
                    glVertex2f(q.x1, old_y);
                    glVertex2f(q.x1, old_y - font.vertical_advance);
                    glVertex2f(q.x0, old_y - font.vertical_advance);
                    glEnd();
                    glEnable(GL_BLEND);
                }
            } else {
                stbtt_GetBakedQuad(font.cdata, 2048, 2048, *ptr-32, &x, &y, &q, 1);
            if(offset == buffer_get_offset(buffer)) {
                glDisable(GL_BLEND);
                glBegin(GL_QUADS);
                glColor4f(0, 1, 0, 1);
                glVertex2f(q.x0, old_y);
                glVertex2f(q.x1, old_y);
                glVertex2f(q.x1, old_y - font.vertical_advance);
                glVertex2f(q.x0, old_y - font.vertical_advance);
                glEnd();
                glEnable(GL_BLEND);
                glBegin(GL_QUADS);
                static float col = (float)0x2f/(float)0xff;
                glColor4f(col, col, col, 1.0f);
                glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y0);
                glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y0);
                glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y1);
                glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y1);
                glEnd();
            } else {
                glBegin(GL_QUADS);
                glColor4f(((float)0xf6/(float)0xff), ((float)0xf3/(float)0xff), ((float)0xe8/(float)0xff), 1.0f);
                glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y0);
                glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y0);
                glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y1);
                glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y1);
                glEnd();
            }
        }
    }else if(*ptr == '\t') {
        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(font.cdata, 2048, 2048, ' '-32, &x, &y, &q, 1);
        stbtt_GetBakedQuad(font.cdata, 2048, 2048, ' '-32, &x, &y, &q, 1);
        stbtt_GetBakedQuad(font.cdata, 2048, 2048, ' '-32, &x, &y, &q, 1);
        stbtt_GetBakedQuad(font.cdata, 2048, 2048, ' '-32, &x, &y, &q, 1);
    }/* else if(*ptr == '\n') {
            y += font.vertical_advance;
            x = 0;
        }*/
        
        offset++;
        ptr++;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}


void charmods_callback(GLFWwindow *window, unsigned int cp, int mods)
{
    if(cp <= 0x7f) {
    buffer_insert(buffer, cp);
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS || action == GLFW_REPEAT) {
    switch(key) {
        case GLFW_KEY_LEFT: {
            buffer_move_left(buffer);
        } break;
        case GLFW_KEY_RIGHT: {
            buffer_move_right(buffer);
        } break;
        case GLFW_KEY_UP: {
            buffer_seek_up(buffer);
        } break;
        case GLFW_KEY_DOWN: {
            buffer_seek_down(buffer);
        } break;
        case GLFW_KEY_ENTER: {
            buffer_insert(buffer, '\n');
        } break;
        case GLFW_KEY_BACKSPACE: {
            buffer_remove(buffer);
        } break;
        case GLFW_KEY_HOME: {
            buffer_seek_home(buffer);
        } break;
        case GLFW_KEY_END: {
            buffer_seek_end(buffer);
        } break;
    }
}
}

int main()
{
    glfwInit();
    
    GLFWwindow *window = glfwCreateWindow(800, 600, "v", 0, 0);
    glfwMakeContextCurrent(window);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCharModsCallback(window, charmods_callback);
        glfwSetKeyCallback(window, key_callback);
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    
    printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
    
    //glClearColor(0, 0, 0, 1);
    float bg = (float)0x2f/(float)0xff;
    glClearColor(bg, bg,bg, 1);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Font font = init_font("liberation-mono.ttf", 18);
    //Font font = init_font("LiberationSans-Regular.ttf", 18);
    
    buffer = buffer_create(256);
    buffer_insert_string(buffer, "float x = 40;\n");
    buffer_insert_string(buffer, "float y = font.vertical_advance;\n");
    buffer_insert_string(buffer, "float z = 13.37;");
    /*FILE *f = fopen("linux_readme.txt", "rb");
    fseek(f, 0, SEEK_END);
    int buffer_size = ftell(f);
    rewind(f);
    unsigned char *buf = malloc(buffer_size+1);
    fread(buf, 1, buffer_size, f);
    fclose(f);
    buf[buffer_size] = 0;
    buffer_insert_string(buffer, buf);
    free(buf);
    buffer_seek(buffer, 0);*/
    
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        draw_buffer(font, buffer);
        
        glfwSwapBuffers(window);
        glfwWaitEvents();
        //glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}