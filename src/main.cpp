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
#include "framework/draw/glDrawProgram.hpp"
#include "framework/sdlWindow.hpp"
#include "matrix.hpp"
#include "vector.hpp"
#include "brick.hpp"

#include "framework/draw/camera.hpp"
#include "framework/draw/mesh.hpp"
#include "framework/draw/glMesh.hpp"
#include "framework/draw/objFormat.hpp"
#include "framework/draw/pngFormat.hpp"

#include <vector>

vec4 cursor;
vec4 target;

static unsigned WIDTH = 640;
static unsigned HEIGHT = 480;
static unsigned BRICKSZ = 1;

int mousex;
int mousey;

class MainApplication : public Application
{
    bool isRunning;
    GLDrawProgram *mainProgram;
    GLFramebuffer *mainBuffer;

    GLDrawProgram *lightProgram;
    GLFramebuffer *lightBuffer;

    std::vector<Brick> bricks;
    Camera *camera;

    public:

    MainApplication()
    {
        window = new SDLWindow(WIDTH, HEIGHT, "BrickSim");
        drawDevice = new GLDrawDevice();
        isRunning = true;

        camera = new Camera;

        Brick::init();

        mainProgram = (GLDrawProgram*) drawDevice->createProgram();
        const char mainvs[] = {
            #include "glsl/mesh.vs.h"
        };
        const char mainfs[] = {
            #include "glsl/mesh.fs.h"
        };

        mainProgram = GLDrawProgram::fromVFShaderStrings(mainvs, mainfs);

        mainBuffer = new GLFramebuffer;
        mainBuffer->appendTarget(new GLTexture(WIDTH, HEIGHT, GLTexture::RGBA8)); //color
        mainBuffer->appendTarget(new GLTexture(WIDTH, HEIGHT, GLTexture::RGBA8I)); //normal
        mainBuffer->appendTarget(new GLTexture(WIDTH, HEIGHT, GLTexture::RGBA32F)); //position
        mainBuffer->setDepth(new GLTexture(WIDTH, HEIGHT, GLTexture::DEPTH32)); // depth
        mainProgram->setDestination(mainBuffer);

        const char lightvs[] = {
            #include "glsl/light.vs.h"
        };

        const char lightfs[] = {
            #include "glsl/light.fs.h"
        };

        lightProgram = GLDrawProgram::fromVFShaderStrings(lightvs, lightfs);
        lightProgram->setAccum(true);
        lightBuffer = new GLFramebuffer;
        lightBuffer->appendTarget(new GLTexture(WIDTH, HEIGHT, GLTexture::RGBA8));
        lightProgram->setDestination(lightBuffer);

        mainProgram->use();
    }

    ~MainApplication()
    {
        delete mainProgram;
    }

    bool tryPlaceBrick()
    {
        Brick b(target, BRICKSZ, BRICKSZ);
        bool collision = false;
        for(int i = 0; i < bricks.size(); i++)
        {
            if(b.collides(bricks[i]))
            {
                printf("collision\n");
                collision = true;
                break;
            }
        }

        if(!collision)
        {
            bricks.push_back(Brick(target, BRICKSZ, BRICKSZ));
            return true;
        }

        return false;
    }

    void input()
    {
        uint8_t *keystate;
        SDL_Event event;

        static bool rclick = 0;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    isRunning = false;
                    break;
                case SDL_MOUSEMOTION:
                    if(rclick)
                    {
                        camera->addRotation(vec4( event.motion.yrel / 100.0f,
                                                 -event.motion.xrel / 100.0f, 0, 0));
                    }
                    SDL_GetMouseState(&mousex, &mousey);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if(event.button.button == SDL_BUTTON_LEFT)
                    {
                        tryPlaceBrick();
                    }
                    if(event.button.button == SDL_BUTTON_RIGHT)
                        rclick = true;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(event.button.button == SDL_BUTTON_RIGHT)
                        rclick = false;
                    break;
            }
        }

        keystate = SDL_GetKeyState(0);
        static bool left = 0;
        static bool right = 0;
        static bool up = 0;
        static bool down = 0;
        static bool space = 0;
        static bool comma = 0;
        static bool period = 0;
        if(keystate[SDLK_LEFT] && !left)
            target.x -= 8;

        if(keystate[SDLK_RIGHT] && !right)
            target.x += 8;

        if(keystate[SDLK_UP] && !up)
            target.z -= 8;

        if(keystate[SDLK_DOWN] && !down)
            target.z += 8;

        if(keystate[SDLK_PERIOD] && !period)
            target.y -= 8;

        if(keystate[SDLK_COMMA] && !comma)
            target.y += 8;

        if(keystate[SDLK_SPACE] && !space)
        {
            tryPlaceBrick();
        }

        if(keystate[SDLK_1])
            BRICKSZ = 1;

        if(keystate[SDLK_2])
            BRICKSZ = 2;

        left = keystate[SDLK_LEFT];
        right = keystate[SDLK_RIGHT];
        down = keystate[SDLK_DOWN];
        up = keystate[SDLK_UP];
        space = keystate[SDLK_SPACE];
        comma = keystate[SDLK_COMMA];
        period = keystate[SDLK_PERIOD];

        cursor = cursor + ((target - cursor) * 0.25f);
        //cursor.print();
        //printf("\n");
        if(cursor.distanceSq(target) > 1000 * 1000)
        {
            cursor = target;
        }
    }

    void applyLighting()
    {
        static float angle = 0.5f;
        //angle += 0.05f;
        mat4 mMatrix = mat4::getTranslation(vec4(5,0,0,0));
        mat4 vMatrix = camera->getView();
        mat4 mvpMatrix = camera->getPerspective() * vMatrix * mMatrix;

        vec4 pos1 = vec4(10.0f, 1000.0f, -500.0f, 1.0f);
        vec4 pos2 = vec4(sin(angle) * -10.0f, 10.0f, -10.0f, 1.0f);
        vec4 pos3 = vec4(sin(angle) * 10.0f + 30.0f, sin(angle) * 10.0f + 10.0f, -10.0f, 1.0f);

        lightProgram->use();
        lightBuffer->bind();
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        vec4 camPos = mat4::getRotation(camera->getRotation()) * (camera->getOffset());
        camPos.z = -camPos.z;
        vec4 lightPos = vec4(10.0f, 1000.0f, -500.0f, 1.0f);
        lightProgram->setUniform("light", lightPos);
        lightProgram->setUniform("camera", camPos);
        lightProgram->bindTexture("t_normal", 0, mainBuffer->getTarget(1));
        lightProgram->bindTexture("t_position", 1, mainBuffer->getTarget(2));
        lightProgram->bindTexture("t_depth", 2, mainBuffer->getDepth());
        ((GLDrawDevice*) drawDevice)->drawFullscreenQuad();

        lightPos = vec4(sin(angle) * -10.0f, 10.0f, -10.0f, 1.0f);
        lightProgram->setUniform("light", lightPos);
        ((GLDrawDevice*) drawDevice)->drawFullscreenQuad();

        lightPos = vec4(sin(angle) * 10.0f + 30.0f, sin(angle) * 10.0f + 10.0f, -10.0f, 1.0f);
        lightProgram->setUniform("light", lightPos);
        ((GLDrawDevice*) drawDevice)->drawFullscreenQuad();
    }

    void drawMesh(GLMesh *mesh, GLTexture *tex, vec4 pos)
    {
        static float angle = 0.5f;
        angle += 0.05f;
        mat4 mMatrix = mat4::getTranslation(pos);
        mat4 vMatrix = camera->getView();

        mat4 mvpMatrix = drawDevice->pMatrix * vMatrix * mMatrix;

        mainProgram->use();
        mainBuffer->bind();

        mainProgram->setUniform("mvpMatrix", mvpMatrix);
        mainProgram->setUniform("mMatrix", mMatrix);

        mainProgram->bindTexture("t_color", 0, tex);
        mainProgram->drawMesh(mesh);
    }

    void drawBrick(GLMesh *mesh, GLTexture *tex, vec4 pos, unsigned w, unsigned h)
    {
        for(int j = 0; j < h; j++)
        {
            for(int i = 0; i < w; i++)
            {
                drawMesh(mesh, tex, pos + vec4(i * 8, 0, j * 8, 0));
            }
        }
    }

    void draw()
    {
        lightBuffer->clear();
        mainBuffer->clear();

        drawBrick(Brick::flatMesh, Brick::plateTexture, vec4(-16 * 8,-8.0, -16 * 8,1), 32, 32);
        for(int i = 0; i < bricks.size(); i++)
        {
            Brick b = bricks[i];
            drawBrick(Brick::fullMesh, Brick::brickTexture, b.position, b.w, b.h );
        }

#define MOUSESUPPORT
#ifdef MOUSESUPPORT
        vec4 MOUSE;
        glReadBuffer(GL_COLOR_ATTACHMENT2);
        glReadPixels(mousex, HEIGHT-mousey, 1, 1, GL_RGBA, GL_FLOAT, &MOUSE.v);
        MOUSE.x = round(MOUSE.x / 8.0f) * 8.0f;
        MOUSE.y = ceil(MOUSE.y / 9.6f) * 9.6f;
        MOUSE.z = round(MOUSE.z / 8.0f) * 8.0f;
        if(MOUSE.x != NAN && MOUSE.y != NAN && MOUSE.z != NAN)
            target = MOUSE;
#endif
        drawBrick(Brick::fullMesh, Brick::brickTexture, cursor, BRICKSZ, BRICKSZ);

        applyLighting();

        ((GLDrawDevice*)drawDevice)->drawToScreen(mainBuffer->getTarget(0),
            mainBuffer->getTarget(1), mainBuffer->getDepth(), lightBuffer->getTarget(0));
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
