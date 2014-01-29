/**
 * main.c
 * brickgl
 * January 21, 2014
 * Brandon Surmanski
 */

#include <stddef.h>
#include <stdbool.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL/SDL.h>

#include "framework/sdlWindow.hpp"
#include "matrix.hpp"
#include "vector.hpp"
#include "brick.h"

bool running = true;
mat4 mat4_perspective;
mat4 mat4_view;

vec4 cube[] = {
    {-1.0f, -1.0f, 0.0f, 1.0f},
    {1.0f, -1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 0.0f, 1.0f},
};
GLuint cubeptr;
GLuint programptr;

SDLWindow *window;

void init()
{
    window = new SDLWindow(640, 480, "Name");

    mat4_perspective = mat4::getFrustum(-1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 512.0f);
    mat4_view = mat4::getIdentity();
    mat4_view.translate(vec4(0.0f, 0.0f, -10.0f));
    //glEnable(GL_DEPTH_TEST);

    programptr = glCreateProgram();

    glGenBuffers(1, &cubeptr); 
    glBindBuffer(GL_ARRAY_BUFFER, cubeptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
}

void destroy()
{
    SDL_Quit();
}

void input()
{
    uint8_t *keystate;
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                running = false;
        }
    }

    keystate = SDL_GetKeyState(0);
}

void update()
{
    SDL_Delay(32);
}

void draw()
{
    window->swapBuffers();
}

int main(int argc, char **argv)
{
    init(); 
    while(running)
    {
        input();
        update();
        draw();
    }
    destroy();
    return 0;
}
