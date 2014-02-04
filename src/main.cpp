/**
 * main.c
 * brickgl
 * January 21, 2014
 * Brandon Surmanski
 */

#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
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

#include "framework/draw/mesh.hpp"
#include "framework/draw/glMesh.hpp"
#include "framework/draw/objMesh.hpp"

GLMesh *cube;
/*

struct VERTEX 
{
    float pos[3];
    short normal[3];
    unsigned short uv[2];
};

VERTEX quad[] =
{
    {{-1.0f, -1.0f, 0.0f}, {0, 0, SHRT_MAX}, {0, 0}},
    {{1.0f, -1.0f, 0.0f}, {0, 0, SHRT_MAX}, {USHRT_MAX, 0}},
    {{1.0f, 1.0f, 0.0f}, {0, 0, SHRT_MAX}, {USHRT_MAX, USHRT_MAX}},
    {{-1.0f, 1.0f, 0.0f}, {0, 0, SHRT_MAX}, {0, USHRT_MAX}},

    {{-1.0f, -1.0f, 1.0f}, {0, 0, SHRT_MIN}, {0, 0}},
    {{1.0f, -1.0f, 1.0f}, {0, 0, SHRT_MIN}, {USHRT_MAX, 0}},
    {{1.0f, 1.0f, 1.0f}, {0, 0, SHRT_MIN}, {USHRT_MAX, USHRT_MAX}},
    {{-1.0f, 1.0f, 1.0f}, {0, 0, SHRT_MIN}, {0, USHRT_MAX}},
};


unsigned short iquad[] =
{
    0,1,2,
    0,2,3,

    6,5,4,
    7,6,4,

    0,1,5,
    0,5,4,

    1,2,6,
    1,6,5,

    3,0,4,
    3,4,7,

    2,3,7,
    2,7,6
};

int nquad = 8;
*/
int niquad = 36;

class MainApplication : public Application
{
    bool isRunning;
    GLDrawProgram *mainProgram;

    public:

    MainApplication()
    {
        window = new SDLWindow(640, 480, "BrickSim");
        drawDevice = new GLDrawDevice();
        isRunning = true;

        Mesh *mesh = loadObjMesh("res/cube.obj");
        cube = new GLMesh(*mesh); //TODO mesh manager
        delete mesh;

        mainProgram = (GLDrawProgram*) drawDevice->createProgram();
        const char vstext[] = {
            #include "glsl/cube.vs.h"
        };
        const char fstext[] = {
            #include "glsl/cube.fs.h"
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
        mainProgram->use();

        /*
        //TEMP
        GLuint quadptr;
        glGenBuffers(1, &quadptr);
        glBindBuffer(GL_ARRAY_BUFFER, quadptr);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX[nquad]), quad, GL_STATIC_DRAW);

        GLuint iquadptr;
        glGenBuffers(1, &iquadptr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iquadptr);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short[niquad]), iquad, GL_STATIC_DRAW);
        */
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

        static float angle = 0.5f;
        angle += 0.05f;
        //mat4 mMatrix = mat4::getRotation(vec4(angle, angle, 0, 0));
        mat4 mMatrix = mat4::getRotation(angle, vec4(0.5, 0.5, 0, 0));
        mMatrix.print();
        printf("\n");
        mat4 vMatrix = mat4::getTranslation(vec4(sin(angle/3.1), cos(angle/4.2), - 10));

        mat4 mvpMatrix = drawDevice->pMatrix * vMatrix * mMatrix;

        glUniformMatrix4fv(glGetUniformLocation(mainProgram->id, "mvpMatrix"), 
                1, GL_FALSE, 
                mvpMatrix.ptr());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        GLuint pos_uint = glGetAttribLocation(mainProgram->id, "position");
        GLuint norm_uint = glGetAttribLocation(mainProgram->id, "normal");
        GLuint uv_uint = glGetAttribLocation(mainProgram->id, "uv");
        glEnableVertexAttribArray(pos_uint);
        //glEnableVertexAttribArray(norm_uint);
        //glEnableVertexAttribArray(uv_uint);
        glVertexAttribPointer(pos_uint, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), 0);
        //glVertexAttribPointer(norm_uint, 3, GL_SHORT, GL_TRUE, sizeof(VERTEX), (void*) 12);
        //glVertexAttribPointer(uv_uint, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(VERTEX), (void*) 18);
        glDrawElements(GL_TRIANGLES, cube->getNElements(), GL_UNSIGNED_SHORT, 0);
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
