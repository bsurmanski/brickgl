/**
 * main.c
 * brickgl
 * January 21, 2014
 * Brandon Surmanski
 */

#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include <SDL/SDL.h>

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#include "framework/application.hpp"
#include "framework/draw/glDrawDevice.hpp"
#include "framework/sdlWindow.hpp"
#include "matrix.hpp"
#include "vector.hpp"
#include "brick.h"

struct VERTEX 
{
    float pos[3];
    unsigned short uv[2];
};

VERTEX quad[] =
{
    {{-1.0f, -1.0f, 0.0f}, {0, 0}},
    {{1.0f, -1.0f, 0.0f}, {USHRT_MAX, 0}},
    {{1.0f, 1.0f, 0.0f}, {USHRT_MAX, USHRT_MAX}},

    {{-1.0f, -1.0f, 0.0f}, {0, 0}},
    {{1.0f, 1.0f, 0.0f}, {USHRT_MAX, USHRT_MAX}},
    {{-1.0f, 1.0f, 0.0f}, {0, USHRT_MAX}},
};

class MainApplication : public Application
{
    bool isRunning;
    GLDrawProgram *mainProgram;

    public:

    MainApplication() : Application(new SDLWindow(640, 480, "MainApp"), new GLDrawDevice())
    {
        isRunning = true;

        mainProgram = (GLDrawProgram*) drawDevice->createProgram();
        const char vstext[] = {
            #include "glsl/test.vs.h"
        };
        const char fstext[] = {
            #include "glsl/test.fs.h"
        };

        GLDrawShader *testvs = GLDrawShader::fromString (
                GLDrawShader::VERTEX_SHADER,
                vstext
                );

        GLDrawShader *testfs = GLDrawShader::fromString (
                GLDrawShader::FRAGMENT_SHADER,
                fstext
                );


        mainProgram->bindStage(0, testvs);
        mainProgram->bindStage(1, testfs);
        mainProgram->clean();

        //TEMP
        GLuint quadptr;
        glGenBuffers(1, &quadptr);
        glBindBuffer(GL_ARRAY_BUFFER, quadptr);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX[6]), quad, GL_STATIC_DRAW);

        glUseProgram(mainProgram->id);
    }

    ~MainApplication()
    {
    
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
                    isRunning = false;
            }
        }

        keystate = SDL_GetKeyState(0);
    }

    void draw()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        GLuint pos_uint = glGetAttribLocation(mainProgram->id, "position");
        GLuint uv_uint = glGetAttribLocation(mainProgram->id, "uv");
        glEnableVertexAttribArray(pos_uint);
        glEnableVertexAttribArray(uv_uint);
        glVertexAttribPointer(pos_uint, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), 0);
        glVertexAttribPointer(uv_uint, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(VERTEX), (void*) 12);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void run()
    {
        while(isRunning)
        {
            input();
            draw();
            SDL_Delay(32);
            SDL_GL_SwapBuffers();
            window->swapBuffers();
        }
    }
};

void update()
{
    SDL_Delay(32);
}

int main(int argc, char **argv)
{
    MainApplication app;
    app.run();
    return 0;
}
