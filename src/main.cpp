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
#include "framework/draw/objFormat.hpp"
#include "framework/draw/pngFormat.hpp"

#include <assert.h>

GLMesh *cube;
GLTexture *cubetex;

class MainApplication : public Application
{
    bool isRunning;
    GLDrawProgram *mainProgram;
    GLFramebuffer *mainBuffer;

    public:

    MainApplication()
    {
        window = new SDLWindow(640, 480, "BrickSim");
        drawDevice = new GLDrawDevice();
        isRunning = true;

        Mesh mesh = objLoad("res/1x1.obj");
        cube = new GLMesh(mesh); //TODO mesh manager
        Image image = pngLoad("res/grass.png");
        assert(image.pixels);
        cubetex = new GLTexture(image);

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

        mainBuffer = new GLFramebuffer;
        mainBuffer->appendTarget(new GLTexture(640, 480, GLTexture::RGBA8));
        mainBuffer->appendTarget(new GLTexture(640, 480, GLTexture::RGBA8));
        mainBuffer->setDepth(new GLTexture(640, 480, GLTexture::DEPTH32));
        mainProgram->setDestination(mainBuffer);

        mainProgram->use();
    }

    ~MainApplication()
    {
        delete mainProgram; 
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
        mat4 mMatrix = mat4::getRotation(angle, vec4(1, 1, 0, 0));
        mat4 vMatrix = mat4::getTranslation(vec4(sin(angle/3.1), cos(angle/4.2), - 100));

        mat4 mvpMatrix = drawDevice->pMatrix * vMatrix * mMatrix;

        mainProgram->use();
        mainBuffer->bind();
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mainProgram->setUniform("mvpMatrix", mvpMatrix);

        mainProgram->bindTexture("t_color", 0, cubetex);
        mainProgram->drawMesh(cube);

        ((GLDrawDevice*)drawDevice)->drawToScreen(mainBuffer->getTarget(0),
            mainBuffer->getTarget(1), mainBuffer->getDepth());
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
